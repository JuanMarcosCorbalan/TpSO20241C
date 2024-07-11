#ifndef HEADERS_CONFIG_H_
#define HEADERS_CONFIG_H_

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include "../../utils/Headers/models.h"

typedef struct t_kernel_config {
	char* ip_memoria;
	char* ip_cpu;
	char* algoritmo_planificacion;
	int puerto_memoria;
	int puerto_cpu_dispatch;
	int puerto_cpu_interrupt;
	int puerto_escucha;
	int quantum;
	int grado_multiprogramacion;
	t_list* recursos;
} t_kernel_config;

extern t_kernel_config* app_config;
extern t_config* app_config_file;

void cargar_config(char* path);
void finalizar_config();

#endif /* HEADERS_CONFIG_H_ */
