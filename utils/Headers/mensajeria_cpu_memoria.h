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

typedef struct dt_marco_memoria {
	uint32_t pid;
	uint32_t numero_pagina;
} dt_marco_memoria;

typedef struct dt_mov {
	uint32_t pid;
	uint32_t valor_registro;
	uint32_t direccion_fisica;
} dt_mov;

typedef struct dt_copy_string {
	uint32_t pid;
	uint32_t direccion_fisica_origen;
	uint32_t direccion_fisica_destino;
	uint32_t tamanio;
} dt_copy_string;

void request_proxima_instruccion(int socket, uint32_t pid, uint32_t program_counter);
dt_proxima_instruccion* deserializar_proxima_instruccion(t_buffer* buffer);

void request_instruccion(int socket, t_instruccion* instruccion);
t_instruccion* deserializar_instruccion(t_buffer* buffer);

void request_solicitud_tamanio_pagina(int socket);

void request_tamanio_pagina(int socket, uint32_t tamanio_pagina);
uint32_t deserializar_tamanio_pagina(int socket);

void request_resize_proceso(int socket, uint32_t pid, uint32_t size);
dt_resize_proceso* deserializar_resize_proceso(t_buffer* buffer);

void request_status_resize_proceso(int socket, uint32_t estado);
uint32_t deserializar_status_resize_proceso(int socket);

void request_marco_memoria(int socket, uint32_t pid, uint32_t numero_pagina);
dt_marco_memoria* deserializar_marco_memoria(t_buffer* buffer);

void request_numero_marco_memoria(int socket, uint32_t numero_marco);
uint32_t deserializar_numero_marco_memoria(int socket);

void request_mov_in(int socket, uint32_t pid, uint32_t direccion_fisica);
void request_mov_out(int socket, uint32_t pid, uint32_t valor_registro, uint32_t direccion_fisica);
dt_mov* deserializar_mov(t_buffer* buffer);

void request_valor_mov_in(int socket, uint32_t valor);
uint32_t deserializar_valor_mov_in(int socket);

void request_status_mov_out(int socket, uint32_t estado);
uint32_t deserializar_status_mov_out(int socket);

void request_copy_string(int socket, uint32_t pid, uint32_t direccion_fisica_origen, uint32_t direccion_fisica_destino, uint32_t tamanio);
dt_copy_string* deserializar_copy_string(t_buffer* buffer);

void request_status_copy_string(int socket, uint32_t estado);
uint32_t deserializar_status_copy_string(int socket);

#endif /* HEADERS_MENSAJERIA_CPU_MEMORIA_H_ */
