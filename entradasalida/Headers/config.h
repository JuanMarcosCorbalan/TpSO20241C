#ifndef HEADERS_CONFIG_H_
#define HEADERS_CONFIG_H_

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>

typedef struct t_io_config {
	char* tipo_infertaz;
	char* ip_kernel;
	char* ip_memoria;
	char* path_base_dialfs;
	int tiempo_unidad_trabajo;
	int puerto_kernel;
	int puerto_memoria;
	int block_size;
	int block_count;
} t_io_config;

extern t_io_config* app_config;

void cargar_config(char* path);

#endif /* HEADERS_CONFIG_H_ */
