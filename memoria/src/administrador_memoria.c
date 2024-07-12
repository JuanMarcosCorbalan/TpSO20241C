#include "../Headers/administrador_memoria.h"

void iniciar_espacio_memoria() {
	espacio_memoria = malloc(app_config->tam_memoria);
	lista_global_marcos = list_create();
	lista_marcos_procesos = list_create();

	size_t tamanio_en_byte = (app_config->tam_memoria/app_config->tam_pagina) / 8;
	puntero_bitmap = malloc(tamanio_en_byte);
	bitarray_memoria = bitarray_create_with_mode(puntero_bitmap, tamanio_en_byte, LSB_FIRST);

	int bits = bitarray_get_max_bit(bitarray_memoria);

    for (int c=0; c<bits; c++)
    {
        bitarray_clean_bit(bitarray_memoria, c);
    }

	pthread_mutex_init(&mutex_acceso_memoria, NULL);
}

uint32_t obtener_numero_marco_disponible() {
	size_t tamanio_bitarray = bitarray_get_max_bit(bitarray_memoria);
	for(int i=0;i<tamanio_bitarray;i++) {
		int valor_bit = bitarray_test_bit(bitarray_memoria, i);
		if(valor_bit == 0) {
			bitarray_set_bit(bitarray_memoria, i);
			return i;
		}
	}
	return 0;
}

t_marcos_proceso* obtener_entrada_marcos_proceso(uint32_t pid) {
	bool buscar_por_pid(void* elem) {
		t_marcos_proceso* aux_marcos = (t_marcos_proceso*) elem;
		return aux_marcos->pid == pid;
	}
	return list_find(lista_marcos_procesos, buscar_por_pid);
}

t_marco* crear_marco(uint32_t pid, uint32_t numero_marco, uint32_t numero_pagina) {
	t_marco* nuevo_marco = malloc(sizeof(t_marco));

	nuevo_marco->numero_marco = numero_marco;
	nuevo_marco->pid = pid;
	nuevo_marco->tamanio_ocupado = 0;
	nuevo_marco->base = numero_marco * app_config->tam_pagina;
	nuevo_marco->limite = nuevo_marco->base + app_config->tam_pagina - 1;
	nuevo_marco->numero_pagina = numero_pagina;

	return nuevo_marco;
}

void iniciar_espacio_memoria_proceso(uint32_t pid) {
	logear_tabla_paginas(pid, 1);
	t_marcos_proceso* marcos_proceso = malloc(sizeof(t_marcos_proceso));
	marcos_proceso->pid = pid;
	marcos_proceso->marcos = list_create();

	uint32_t numero_marco_disponible = obtener_numero_marco_disponible();
	t_marco* nuevo_marco = crear_marco(pid, numero_marco_disponible, 0);

	list_add(marcos_proceso->marcos, nuevo_marco);
	list_add(lista_marcos_procesos, marcos_proceso);
	list_add(lista_global_marcos, nuevo_marco);
}

uint32_t buscar_numero_marco_por_pagina(uint32_t pid, uint32_t numero_pagina) {
	bool search(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		return aux_marco->pid == pid && aux_marco->numero_pagina == numero_pagina;
	}

	t_marco* marco = list_find(lista_global_marcos, search);
	logear_acceso_tabla_paginas(pid, numero_pagina, marco->numero_marco);
	return marco->numero_marco;
}

void liberar_marcos_proceso(uint32_t pid) {
	t_marcos_proceso* marcos_proceso = obtener_entrada_marcos_proceso(pid);

	bool buscar_por_pid(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		return aux_marco->pid == pid;
	}

	void eliminar_marco(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		list_remove_element(marcos_proceso->marcos, aux_marco);
		bitarray_clean_bit(bitarray_memoria, aux_marco->numero_marco);
		free(aux_marco);
	}

	logear_tabla_paginas(pid, marcos_proceso->marcos->elements_count);
	list_remove_and_destroy_all_by_condition(lista_global_marcos, buscar_por_pid, eliminar_marco);
	list_destroy(marcos_proceso->marcos);
	free(marcos_proceso);
}

t_marco* obtener_ultimo_marco(uint32_t pid) {
	t_marcos_proceso* marcos_proceso = obtener_entrada_marcos_proceso(pid);
	return list_get(marcos_proceso->marcos, marcos_proceso->marcos->elements_count-1);
}

void agregar_marco_proceso(t_marcos_proceso* marcos_proceso) {
	t_marco* ultimo_marco = obtener_ultimo_marco(marcos_proceso->pid);
	uint32_t numero_marco_disponible = obtener_numero_marco_disponible();
	t_marco* nuevo_marco = crear_marco(marcos_proceso->pid, numero_marco_disponible, ultimo_marco->numero_pagina+1);

	list_add(lista_global_marcos, nuevo_marco);
	list_add(marcos_proceso->marcos, nuevo_marco);
}

uint32_t operar_resize_proceso(uint32_t pid, uint32_t tamanio_nuevo) {
	t_marcos_proceso* marcos_proceso = obtener_entrada_marcos_proceso(pid);
	uint32_t marcos_asignados = marcos_proceso->marcos->elements_count;
	uint32_t marcos_nuevos = tamanio_nuevo / app_config->tam_pagina;

	if(tamanio_nuevo % app_config->tam_pagina)
		marcos_nuevos += 1;

	if(marcos_asignados > marcos_nuevos) {
		logear_reduccion_proceso(pid, marcos_asignados * app_config->tam_pagina, marcos_nuevos * app_config->tam_pagina);

		for(int i=0; i<marcos_nuevos; i++) {
			t_marco* ultimo_marco = obtener_ultimo_marco(pid);
			bitarray_clean_bit(bitarray_memoria, ultimo_marco->numero_marco);
			list_remove_element(lista_global_marcos, ultimo_marco);
			list_remove_element(marcos_proceso->marcos, ultimo_marco);
			free(ultimo_marco);
		}
	}
	else {
		logear_ampliacion_proceso(pid, marcos_asignados * app_config->tam_pagina, marcos_nuevos * app_config->tam_pagina);
		uint32_t marcos_disponibles = (app_config->tam_memoria/app_config->tam_pagina) - lista_global_marcos->elements_count;

		if(marcos_nuevos > marcos_disponibles)
			return 0;

		for(int i=0; i<marcos_nuevos; i++) {
			agregar_marco_proceso(marcos_proceso);
		}
	}
	return 1;
}

t_marco* obtener_marco_por_id(uint32_t numero_marco) {
	bool search(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		return (aux_marco->numero_marco == numero_marco);
	}
	return list_find(lista_global_marcos, search);
}

void* lectura_memoria(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio) {
	logear_lectura_espacio_usuario(pid, direccion_fisica, tamanio);

	t_marcos_proceso* marcos_proceso = obtener_entrada_marcos_proceso(pid);
    uint32_t marco_lectura = floor(direccion_fisica/app_config->tam_pagina);
    t_marco* marco_solicitado = obtener_marco_por_id(marco_lectura);

    void* valor_lectura = malloc(tamanio);

    if(marco_solicitado->pid != pid || marcos_proceso->marcos->elements_count * app_config->tam_pagina < tamanio)
    	return 0;

    uint32_t disponible_a_leer = ((marco_solicitado->numero_marco + 1)* app_config->tam_pagina) - direccion_fisica;

    if(tamanio <= disponible_a_leer) {
    	memcpy(valor_lectura, espacio_memoria + direccion_fisica, tamanio);
    	return valor_lectura;
    }

    uint32_t tamanio_leido = 0;
    uint32_t tamanio_restante = tamanio;
    uint32_t offset_inicial = disponible_a_leer;
    memcpy(valor_lectura, espacio_memoria + direccion_fisica, offset_inicial);
    tamanio_leido += offset_inicial;
    tamanio_restante -= offset_inicial;
    uint32_t pagina_actual = marco_solicitado->numero_pagina + 1;

    while(tamanio_restante > 0) {
    	t_marco* sgte_marco = list_get(marcos_proceso->marcos, pagina_actual);

    	if(sgte_marco == NULL || sgte_marco->pid != pid)
    		return 0;

    	if(tamanio_restante > app_config->tam_pagina) {
    		memcpy(valor_lectura + tamanio_leido, espacio_memoria + sgte_marco->base, app_config->tam_pagina);
    	    tamanio_leido += app_config->tam_pagina;
    		tamanio_restante -= app_config->tam_pagina;
    		pagina_actual += 1;
    	}
    	else {
    		memcpy(valor_lectura + tamanio_leido, espacio_memoria + sgte_marco->base, tamanio_restante);
    		tamanio_restante = 0;
    	}
    }

	return valor_lectura;
}

uint32_t escritura_memoria(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio, void* valor) {
	logear_escritura_espacio_usuario(pid, direccion_fisica, tamanio);

	t_marcos_proceso* marcos_proceso = obtener_entrada_marcos_proceso(pid);
    uint32_t marco_escritura = floor(direccion_fisica/app_config->tam_pagina);
    t_marco* marco_solicitado = obtener_marco_por_id(marco_escritura);

    if(marco_solicitado->pid != pid || marcos_proceso->marcos->elements_count * app_config->tam_pagina < tamanio)
    	return 0;

    uint32_t disponible_a_escribir = ((marco_solicitado->numero_marco + 1)* app_config->tam_pagina) - direccion_fisica;

    if(tamanio <= disponible_a_escribir) {
    	memcpy(espacio_memoria + direccion_fisica, valor, tamanio);
    	return 1;
    }

    uint32_t tamanio_grabado = 0;
    uint32_t tamanio_restante = tamanio;
    uint32_t offset_inicial = disponible_a_escribir;
    memcpy(espacio_memoria + direccion_fisica, valor, offset_inicial);
    tamanio_grabado += offset_inicial;
    tamanio_restante -= offset_inicial;
    uint32_t pagina_actual = marco_solicitado->numero_pagina + 1;

    while(tamanio_restante > 0) {
    	t_marco* sgte_marco = list_get(marcos_proceso->marcos, pagina_actual);

    	if(sgte_marco == NULL || sgte_marco->pid != pid)
    		return 0;

    	if(tamanio_restante > app_config->tam_pagina) {
    		memcpy(espacio_memoria + sgte_marco->base, valor + tamanio_grabado, app_config->tam_pagina);
    	    tamanio_grabado += app_config->tam_pagina;
    		tamanio_restante -= app_config->tam_pagina;
    		pagina_actual += 1;
    	}
    	else {
    		memcpy(espacio_memoria + sgte_marco->base, valor + tamanio_grabado, tamanio_restante);
    		tamanio_restante = 0;
    	}
    }

	return 1;
}

uint32_t operar_copy_string(uint32_t pid, uint32_t origen, uint32_t destino, uint32_t tamanio) {
	logear_lectura_espacio_usuario(pid, origen, tamanio);
	void* valor_lectura = lectura_memoria(pid, origen, tamanio);
	logear_escritura_espacio_usuario(pid, destino, tamanio);
	uint32_t resultado = escritura_memoria(pid, destino, tamanio, valor_lectura);
	free(valor_lectura);
	return resultado;
}

