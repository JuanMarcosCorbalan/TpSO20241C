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

typedef struct t_recursoSistema {
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
	int pid;
	int program_counter;
	int quantum;
	t_registros_cpu* registros_cpu;
} t_pcb;

#endif /* HEADERS_MODELS_H_ */
