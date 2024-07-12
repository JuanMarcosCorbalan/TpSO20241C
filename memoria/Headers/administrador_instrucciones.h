#ifndef HEADERS_ADMINISTRADOR_INSTRUCCIONES_H_
#define HEADERS_ADMINISTRADOR_INSTRUCCIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <commons/collections/list.h>
#include "../../utils/Headers/models.h"
#include "../../utils/Headers/enums.h"
#include <commons/string.h>

typedef struct t_instrucciones_proceso{
	uint32_t pid;
	t_list* instrucciones;
} t_instrucciones_proceso;

extern t_list* instrucciones_procesos;

void cargar_instrucciones(uint32_t pid, char* path);
t_instruccion* obtener_instruccion(uint32_t pid, uint32_t program_counter);
void remover_instrucciones(uint32_t pid);
void eliminar_instrucciones();

#endif /* HEADERS_ADMINISTRADOR_INSTRUCCIONES_H_ */
