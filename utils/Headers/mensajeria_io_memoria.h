#ifndef HEADERS_MENSAJERIA_IO_MEMORIA_H_
#define HEADERS_MENSAJERIA_IO_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "communication.h"
#include "models.h"
#include "enums.h"

typedef struct dt_cpu_escritura {
	uint32_t pid;
	uint32_t direccion_fisica;
	uint32_t tamanio_read_write;
	char* valor_std;
} dt_rw_memoria;

typedef struct dt_rw_fs {
	uint32_t pid;
	uint32_t direccion_fisica;
	uint32_t tamanio_lectura;
	uint32_t tamanio_valor;
	char* valor;
}dt_rw_fs ;

void request_escritura_memoria(int socket, uint32_t pid, uint32_t direccion_fisica, char* valor_std);
dt_rw_memoria* deserializar_escritura_memoria(t_buffer* buffer);

void request_lectura_memoria(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio_lectura);
dt_rw_memoria* deserializar_lectura_memoria(t_buffer* buffer);

void request_resultado_lectura_memoria(int socket, char* valor_lectura);
void request_status_escritura_memoria(int socket, uint32_t estado);

uint32_t deserializar_status_escritura_memoria(int socket);
char* deserializar_resultado_lectura_memoria(int socket);


void request_fs_lectura(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio_lectura);
void request_fs_escritura(int socket, uint32_t pid, uint32_t direccion_fisica, char* valor);
dt_rw_fs* deserializar_rw_fs(t_buffer* buffer);

void request_valor_fs_lectura(int socket, char* valor_lectura);
char* deserializar_valor_fs_lectura(int socket);

#endif /* HEADERS_MENSAJERIA_IO_MEMORIA_H_ */
