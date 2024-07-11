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
	int retraso_compactacion;
} t_io_config;

extern t_io_config* app_config;
extern t_config* archivo_config;

void cargar_config(char* path);
void finalizar_config();

#endif /* HEADERS_CONFIG_H_ */
