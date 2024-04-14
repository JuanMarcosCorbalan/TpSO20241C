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

void request_proxima_instruccion(int socket, uint32_t pid, uint32_t program_counter);
dt_proxima_instruccion* deserializar_proxima_instruccion(t_buffer* buffer);

void request_instruccion(int socket, t_instruccion* instruccion);
t_instruccion* deserializar_instruccion(t_buffer* buffer);

#endif /* HEADERS_MENSAJERIA_CPU_MEMORIA_H_ */
