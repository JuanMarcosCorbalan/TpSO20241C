#include "../Headers/administrador_memoria.h"

uint32_t obtener_marco_disponible() {
	size_t tamanio_bitarray = bitarray_get_max_bit(bitarray_memoria);
	for(int i=0;i<tamanio_bitarray;i++) {
		int valor_bit = bitarray_test_bit(bitarray_memoria, i);
		if(valor_bit == 0) {
			bitarray_set_bit(bitarray_memoria, i);
			return i+1;
		}
	}
	return 0;
}

t_marco* crear_marco(uint32_t pid, uint32_t numero_marco, uint32_t numero_pagina) {
	t_marco* nuevo_marco = malloc(sizeof(t_marco));

	nuevo_marco->numero_marco = numero_marco;
	nuevo_marco->pid = pid;
	nuevo_marco->tamanio_ocupado = 0;
	nuevo_marco->base = (numero_marco-1) * app_config->tam_pagina;
	nuevo_marco->limite = nuevo_marco->base + app_config->tam_pagina;
	nuevo_marco->numero_pagina = numero_pagina;

	return nuevo_marco;
}

void iniciar_marco_proceso(uint32_t pid) {
	uint32_t numero_marco_disponible = obtener_marco_disponible();
	t_marco* nuevo_marco = crear_marco(pid, numero_marco_disponible, 1);
	list_add(tabla_paginas, nuevo_marco);
}

uint32_t buscar_numero_marco_por_pagina(uint32_t pid, uint32_t numero_pagina) {
	bool search(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		return aux_marco->pid == pid && aux_marco->numero_pagina == numero_pagina;
	}
	t_marco* marco = list_find(tabla_paginas, search);
	return (marco == NULL) ? 0 : marco->numero_marco;
}

void liberar_marcos_proceso(uint32_t pid) {
	bool buscar_por_pid(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		return aux_marco->pid == pid;
	}

	void eliminar_marco(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		bitarray_clean_bit(bitarray_memoria, aux_marco->numero_marco-1);
		free(aux_marco);
	}

	list_remove_and_destroy_all_by_condition(tabla_paginas, buscar_por_pid, eliminar_marco);
}

t_marco* obtener_ultimo_marco(uint32_t pid) {
	t_marco* marco = NULL;

	void ultimo_marco(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		if((marco == NULL && aux_marco->pid == pid) || (aux_marco->pid == pid && aux_marco->numero_pagina > marco->numero_pagina))
			marco = aux_marco;
	}
	list_iterate(tabla_paginas, ultimo_marco);
	return marco;
}

t_marco* agregar_marco_proceso(uint32_t pid) {
	uint32_t numero_marco_disponible = obtener_marco_disponible();
	t_marco* ultimo_marco = obtener_ultimo_marco(pid);
	t_marco* nuevo_marco = crear_marco(pid, numero_marco_disponible, ultimo_marco->numero_pagina+1);
	list_add(tabla_paginas, nuevo_marco);
	return nuevo_marco;
}

void liberar_ultimo_marco_proceso(uint32_t pid) {
	t_marco* marco = obtener_ultimo_marco(pid);
	bitarray_clean_bit(bitarray_memoria, marco->numero_marco-1);
	list_remove_element(tabla_paginas, marco);
	free(marco);
}

uint32_t operar_resize_proceso(uint32_t pid, uint32_t tamanio_nuevo) {
	t_marco* marco = obtener_ultimo_marco(pid);
	uint32_t tamanio_anterior = marco->numero_pagina * app_config->tam_pagina;
	uint32_t diferencia_tamanios = 0;
	uint32_t diferencia_paginas = 0;

	if(tamanio_anterior > tamanio_nuevo) {
		// HAY QUE REDUCIR
		diferencia_tamanios = tamanio_anterior - tamanio_nuevo;
		diferencia_paginas = diferencia_tamanios / app_config->tam_pagina;

		for(int i=0; i<diferencia_paginas; i++) {
			liberar_ultimo_marco_proceso(pid);
		}
	}
	else {
		// HAY QUE AGRANDAR
		diferencia_tamanios = tamanio_nuevo - tamanio_anterior;
		diferencia_paginas = diferencia_tamanios / app_config->tam_pagina;
		uint32_t paginas_disponibles = (app_config->tam_memoria/app_config->tam_pagina) - tabla_paginas->elements_count;

		if(diferencia_paginas > paginas_disponibles)
			return 0;

		for(int i=0; i<diferencia_paginas; i++) {
			agregar_marco_proceso(pid);
		}
	}
	return 1;
}



