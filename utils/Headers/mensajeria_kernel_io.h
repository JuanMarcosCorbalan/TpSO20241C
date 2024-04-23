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
} dt_io_sleep;

typedef struct dt_exec_io_std {
	uint32_t pid;
	uint32_t direccion_fisica;
	uint32_t tamanio;
} dt_io_std;

void request_iniciar_interfaz(int socket, char* nombre);
dt_iniciar_interfaz* deserializar_iniciar_interfaz(t_buffer* buffer);

void request_validar_interfaz(int socket, char* instruccion);
char* deserializar_validar_interfaz(t_buffer* buffer);

void request_estado_validacion_instruccion(int socket, uint8_t estado_validacion);
uint8_t deserializar_estado_validacion_instruccion(t_buffer* buffer);

void request_io_gen_sleep(int socket, char* instruccion, uint32_t unidad_trabajo, uint32_t pid);
dt_io_sleep* deserializar_io_gen_sleep(t_buffer* buffer);

void request_desbloquear_proceso(int socket, uint32_t pid);
uint32_t deserializar_desbloquear_proceso(t_buffer* buffer);

void request_io_stdin_read(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio);
void request_io_stdout_write(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio);
dt_io_std* deserializar_io_std(t_buffer* buffer);

#endif /* HEADERS_MENSAJERIA_KERNEL_IO_H_ */
