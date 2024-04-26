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
	nuevo_marco->base = numero_marco * app_config->tam_pagina;
	nuevo_marco->limite = nuevo_marco->base + app_config->tam_pagina;
	nuevo_marco->numero_pagina = numero_pagina;

	return nuevo_marco;
}

void iniciar_marco_proceso(uint32_t pid) {
	uint32_t numero_marco_disponible = obtener_marco_disponible();
	t_marco* nuevo_marco = crear_marco(pid, numero_marco_disponible, 1);
	list_add(tabla_paginas, nuevo_marco);
}

uint32_t buscar_numero_marco(uint32_t pid, uint32_t numero_pagina) {
	bool buscar_marco_por_pagina(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		return aux_marco->pid == pid && aux_marco->numero_pagina == numero_pagina;
	}
	t_marco* marco = list_find(tabla_paginas, buscar_marco_por_pagina);
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
