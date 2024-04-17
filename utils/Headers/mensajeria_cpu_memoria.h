#ifndef HEADERS_MENSAJERIA_CPU_MEMORIA_H_
#define HEADERS_MENSAJERIA_CPU_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "communication.h"
#include "models.h"
#include "enums.h"

typedef struct dt_proxima_instruccion {
	uint32_t pid;
	uint32_t program_counter;
} dt_proxima_instruccion;

typedef struct dt_resize_proceso {
	uint32_t pid;
	uint32_t size;
} dt_resize_proceso;

void request_proxima_instruccion(int socket, uint32_t pid, uint32_t program_counter);
dt_proxima_instruccion* deserializar_proxima_instruccion(t_buffer* buffer);

void request_instruccion(int socket, t_instruccion* instruccion);
t_instruccion* deserializar_instruccion(t_buffer* buffer);

void request_solicitud_tamanio_pagina(int socket);
void request_tamanio_pagina(int socket, uint32_t tamanio_pagina);
uint32_t deserializar_tamanio_pagina(int socket);

void request_resize_proceso(int socket, uint32_t pid, uint32_t size);
dt_resize_proceso* deserializar_resize_proceso(int socket);

void request_status_resize_proceso(int socket, uint32_t estado);
uint32_t deserializar_status_resize_proceso(int socket);

#endif /* HEADERS_MENSAJERIA_CPU_MEMORIA_H_ */
