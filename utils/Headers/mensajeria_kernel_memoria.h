#ifndef HEADERS_MENSAJERIA_KERNEL_MEMORIA_H_
#define HEADERS_MENSAJERIA_KERNEL_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "communication.h"
#include "models.h"
#include "enums.h"

typedef struct dt_iniciar_proceso{
	char* path;
	uint32_t pid;
	uint32_t tamanio_path;
} dt_iniciar_proceso;

void request_iniciar_proceso(int socket, t_pcb* proceso);
dt_iniciar_proceso* deserializar_iniciar_proceso(t_buffer* buffer);

void request_proceso_bloqueado(int socket, uint32_t estado);
uint32_t deserializar_proceso_bloqueado(int socket);

void request_finalizar_proceso(int socket, uint32_t pid);
uint32_t deserializar_finalizar_proceso(t_buffer* buffer);

#endif /* HEADERS_MENSAJERIA_KERNEL_MEMORIA_H_ */
