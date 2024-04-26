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

t_marco* crear_entrada(uint32_t pid, uint32_t numero_marco) {
	t_marco* nuevo_marco = malloc(sizeof(t_marco));

	nuevo_marco->id = numero_marco;
	nuevo_marco->pid = pid;
	nuevo_marco->base = numero_marco * app_config->tam_pagina;
	nuevo_marco->limite = nuevo_marco->base + app_config->tam_pagina;
	nuevo_marco->sgte_marco = 0;

	return nuevo_marco;
}

void iniciar_marco_proceso(uint32_t pid) {
	uint32_t numero_marco_disponible = obtener_marco_disponible();
	t_marco* nuevo_marco = crear_entrada(pid, numero_marco_disponible);
	list_add(tabla_paginas, nuevo_marco);
}

void liberar_marcos_proceso(uint32_t pid) {
	// LIBERO MARCOS
	bool buscar_por_pid(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		if(aux_marco->pid == pid)
			return 1;
		return 0;
	}

	void eliminar_marco(void* elem) {
		t_marco* aux_marco = (t_marco*) elem;
		bitarray_clean_bit(bitarray_memoria, aux_marco->id-1);
		free(aux_marco);
	}

	list_remove_and_destroy_all_by_condition(tabla_paginas, buscar_por_pid, eliminar_marco);
}
