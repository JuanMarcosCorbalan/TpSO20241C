#ifndef HEADERS_MENSAJERIA_KERNEL_CPU_H_
#define HEADERS_MENSAJERIA_KERNEL_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "communication.h"
#include "models.h"
#include "enums.h"

typedef struct dt_interrumpir_proceso {
	uint32_t pid;
	uint8_t motivo;
} dt_interrumpir_proceso;

typedef struct dt_contexto_proceso {
	uint32_t pid;
	uint32_t program_counter;
	uint32_t quantum;
	uint32_t quantum_ejecutados;
	uint8_t algoritmo;
	uint8_t estado;
	uint8_t motivo_blocked;
	uint8_t motivo_exit;
	t_registros_cpu* registros_cpu;
} dt_contexto_proceso;

typedef struct dt_sleep_proceso {
	dt_contexto_proceso* contexto_proceso;
	uint32_t tamanio_nombre_interfaz;
	uint32_t unidad_trabajo;
	char* nombre_interfaz;
} dt_sleep_proceso;

void request_interrumpir_proceso(int socket, uint32_t pid, uint8_t motivo);
dt_interrumpir_proceso* deserializar_interrumpir_proceso(t_buffer* buffer);

void request_desalojo_proceso(int socket, dt_contexto_proceso* contexto_proceso);
void request_exit_proceso(int socket, dt_contexto_proceso* contexto_proceso);
void request_contexto_proceso(int socket, t_pcb* proceso);
dt_contexto_proceso* deserializar_contexto_proceso(t_buffer* buffer);

void request_sleep_proceso(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, uint32_t unidad_trabajo);
dt_sleep_proceso* deserializar_sleep_proceso(t_buffer* buffer);


#endif /* HEADERS_MENSAJERIA_KERNEL_CPU_H_ */
