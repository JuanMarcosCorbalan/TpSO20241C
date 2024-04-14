#ifndef HEADERS_MENSAJERIA_KERNEL_IO_H_
#define HEADERS_MENSAJERIA_KERNEL_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "communication.h"
#include "models.h"
#include "enums.h"

typedef struct dt_iniciar_interfaz {
	uint32_t tamanio_nombre;
	char* nombre;
} dt_iniciar_interfaz;

typedef struct dt_instruccion {
	uint32_t pid;
	uint32_t tamanio_instruccion;
	uint32_t unidad_trabajo;
	char* instruccion;
} dt_instruccion;

void request_iniciar_interfaz(int socket, char* nombre);
dt_iniciar_interfaz* deserializar_iniciar_interfaz(t_buffer* buffer);

void request_validar_interfaz(int socket, char* instruccion);
char* deserializar_validar_interfaz(t_buffer* buffer);

void request_estado_validacion_instruccion(int socket, uint8_t estado_validacion);
uint8_t deserializar_estado_validacion_instruccion(t_buffer* buffer);

void request_ejecutar_instruccion(int socket, char* instruccion, uint32_t unidad_trabajo, uint32_t pid);
dt_instruccion* deserializar_ejecutar_instruccion(t_buffer* buffer);

void request_desbloquear_proceso(int socket, uint32_t pid);
uint32_t deserializar_desbloquear_proceso(t_buffer* buffer);

#endif /* HEADERS_MENSAJERIA_KERNEL_IO_H_ */
