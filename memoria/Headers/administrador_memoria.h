#ifndef HEADERS_ADMINISTRADOR_MEMORIA_H_
#define HEADERS_ADMINISTRADOR_MEMORIA_H_

#include "config.h"
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <math.h>
#include <pthread.h>

typedef struct t_marco {
	uint32_t numero_marco;
	uint32_t numero_pagina;
	uint32_t pid;
	uint32_t base;
	uint32_t limite;
	uint32_t tamanio_ocupado;
} t_marco;

typedef struct t_marcos_proceso {
	uint32_t pid;
	t_list* marcos;
} t_marcos_proceso;

extern t_memoria_config* app_config;
extern t_log* app_log;
extern t_list* lista_global_marcos;
extern t_list* lista_marcos_procesos;
extern t_bitarray* bitarray_memoria;
extern void* espacio_memoria;
extern void* bits_memoria;
extern void* puntero_bitmap;
extern pthread_mutex_t mutex_acceso_memoria;

void iniciar_espacio_memoria();
void iniciar_espacio_memoria_proceso(uint32_t pid);
void liberar_marcos_proceso(uint32_t pid);
uint32_t buscar_numero_marco_por_pagina(uint32_t pid, uint32_t numero_pagina);
uint32_t operar_resize_proceso(uint32_t pid, uint32_t tamanio_nuevo);
void agregar_marco_proceso(t_marcos_proceso* marcos_proceso);
uint32_t operar_copy_string(uint32_t pid, uint32_t origen, uint32_t destino, uint32_t tamanio);
t_marcos_proceso* obtener_entrada_marcos_proceso(uint32_t pid);
void* lectura_memoria(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio);
uint32_t escritura_memoria(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio, void* valor);

#endif /* HEADERS_ADMINISTRADOR_MEMORIA_H_ */
