#ifndef HEADERS_MODELS_H_
#define HEADERS_MODELS_H_

#include <stdint.h>
#include <commons/collections/list.h>
#include "enums.h"

typedef struct {
    uint32_t size;
    void* stream;
} t_buffer;

typedef struct {
    uint8_t codigo_operacion;
    t_buffer* buffer;
} t_paquete;

typedef struct t_recurso_sistema {
	char* nombre;
	int cantidad;
	t_list* solicitud;
	t_list* asignacion;
} t_recurso_sistema;

typedef struct t_registros_cpu {
	uint32_t PC;
	uint32_t SI;
	uint32_t DI;
	uint32_t EAX;
	uint32_t EBX;
	uint32_t ECX;
	uint32_t EDX;
	uint8_t AX;
	uint8_t BX;
	uint8_t CX;
	uint8_t DX;
} t_registros_cpu;

typedef struct t_pcb {
	uint32_t pid;
	uint32_t program_counter;
	uint32_t quantum;
	uint32_t quantum_ejecutados;
	uint8_t algoritmo;
	uint8_t estado;
	uint8_t motivo_blocked;
	uint8_t motivo_exit;
	char* path_instrucciones;
	t_registros_cpu* registros_cpu;
} t_pcb;

typedef struct t_instruccion {
	uint32_t tamanio_instruccion;
	uint32_t tamanio_parametro_1;
	uint32_t tamanio_parametro_2;
	uint32_t tamanio_parametro_3;
	uint32_t tamanio_parametro_4;
	uint32_t tamanio_parametro_5;
	char* instruccion;
	char* parametro_1;
	char* parametro_2;
	char* parametro_3;
	char* parametro_4;
	char* parametro_5;
} t_instruccion;

typedef struct t_entrada {
	uint32_t pid;
	uint32_t pagina;
	uint32_t marco;
	char* ultimo_uso;
} t_entrada;

#endif /* HEADERS_MODELS_H_ */
