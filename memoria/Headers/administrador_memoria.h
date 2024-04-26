#ifndef HEADERS_ADMINISTRADOR_MEMORIA_H_
#define HEADERS_ADMINISTRADOR_MEMORIA_H_

#include "config.h"
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <commons/bitarray.h>

typedef struct t_marco {
	uint32_t numero_marco;
	uint32_t numero_pagina;
	uint32_t pid;
	uint32_t base;
	uint32_t limite;
	uint32_t tamanio_ocupado;
} t_marco;

extern t_memoria_config* app_config;
extern t_log* app_log;
extern t_list* tabla_paginas;
extern void* espacio_memoria;
extern void* bits_memoria;
extern t_bitarray* bitarray_memoria;

void iniciar_marco_proceso(uint32_t pid);
void liberar_marcos_proceso(uint32_t pid);
uint32_t buscar_numero_marco(uint32_t pid, uint32_t numero_pagina);

#endif /* HEADERS_ADMINISTRADOR_MEMORIA_H_ */
