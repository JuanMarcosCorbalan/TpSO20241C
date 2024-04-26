#ifndef HEADERS_ADMINISTRADOR_MEMORIA_H_
#define HEADERS_ADMINISTRADOR_MEMORIA_H_

#include "config.h"
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <commons/bitarray.h>

typedef struct t_marco {
	uint32_t id;
	uint32_t pid;
	uint32_t base;
	uint32_t limite;
	uint32_t sgte_marco;
} t_marco;

extern t_memoria_config* app_config;
extern t_log* app_log;
extern t_list* tabla_paginas;
extern void* espacio_memoria;
extern void* bits_memoria;
extern t_bitarray* bitarray_memoria;

void iniciar_marco_proceso(uint32_t pid);
void liberar_marcos_proceso(uint32_t pid);

#endif /* HEADERS_ADMINISTRADOR_MEMORIA_H_ */
