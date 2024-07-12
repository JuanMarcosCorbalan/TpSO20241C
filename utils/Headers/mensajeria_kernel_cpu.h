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

typedef struct dt_recurso_proceso {
	dt_contexto_proceso* contexto_proceso;
	uint32_t tamanio_nombre_recurso;
	char* nombre_recurso;
} dt_recurso_proceso;

typedef struct dt_io_std {
	dt_contexto_proceso* contexto_proceso;
	char* nombre_interfaz;
	uint32_t tamanio_nombre_interfaz;
	uint32_t direccion_fisica;
	uint32_t tamanio;
} dt_std;

typedef struct dt_io_file {
	dt_contexto_proceso* contexto_proceso;
	char* nombre_archivo;
	uint32_t tamanio_nombre_archivo;
	char* nombre_interfaz;
	uint32_t tamanio_nombre_interfaz;
} dt_io_file;

typedef struct dt_io_file_truncate {
	dt_contexto_proceso* contexto_proceso;
	char* nombre_archivo;
	uint32_t tamanio_nombre_archivo;
	char* nombre_interfaz;
	uint32_t tamanio_nombre_interfaz;
	uint32_t tamanio_nuevo;
} dt_io_file_truncate;

typedef struct dt_io_file_rw {
	dt_contexto_proceso* contexto_proceso;
	char* nombre_archivo;
	uint32_t tamanio_nombre_archivo;
	char* nombre_interfaz;
	uint32_t tamanio_nombre_interfaz;
	uint32_t registro_direccion;
	uint32_t registro_tamanio;
	uint32_t registro_puntero;
} dt_io_file_rw;

void request_interrumpir_proceso_bloquear(int socket, uint32_t pid, uint8_t motivo);
void request_interrumpir_proceso_exit(int socket, uint32_t pid, uint8_t motivo);
dt_interrumpir_proceso* deserializar_interrumpir_proceso(t_buffer* buffer);

void request_desalojo_proceso(int socket, dt_contexto_proceso* contexto_proceso);
void request_exit_proceso(int socket, dt_contexto_proceso* contexto_proceso);
void request_contexto_proceso(int socket, t_pcb* proceso);
dt_contexto_proceso* deserializar_contexto_proceso(t_buffer* buffer);

void request_sleep_proceso(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, uint32_t unidad_trabajo);
dt_sleep_proceso* deserializar_sleep_proceso(t_buffer* buffer);

void request_signal_recurso(int socket, dt_contexto_proceso* contexto, char* nombre_recurso);
void request_wait_recurso(int socket, dt_contexto_proceso* contexto, char* nombre_recurso);
dt_recurso_proceso* deserializar_recurso(t_buffer* buffer);

void request_desbloquear_cpu(int socket, uint32_t pid);
uint32_t deserializar_desbloquear_cpu(int socket);

void request_stdin_read(int socket, char* nombre_interfaz, dt_contexto_proceso* contexto, uint32_t direccion_fisica, uint32_t tamanio);
void request_stdout_write(int socket, char* nombre_interfaz, dt_contexto_proceso* contexto, uint32_t direccion_fisica, uint32_t tamanio);
dt_std* deserializar_std(t_buffer* buffer);

void request_io_create(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, char* nombre_archivo);
void request_io_delete(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, char* nombre_archivo);
dt_io_file* deserializar_io_file(t_buffer* buffer);

void request_io_truncate(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, char* nombre_archivo, uint32_t tamanio_nuevo);
dt_io_file_truncate* deserializar_io_truncate(t_buffer* buffer);

void request_io_read(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, char* nombre_archivo, uint32_t registro_direccion, uint32_t registro_tamanio, uint32_t registro_puntero_archivo);
void request_io_write(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, char* nombre_archivo, uint32_t registro_direccion, uint32_t registro_tamanio, uint32_t registro_puntero_archivo);
dt_io_file_rw* deserializar_io_file_rw(t_buffer* buffer);

#endif /* HEADERS_MENSAJERIA_KERNEL_CPU_H_ */
