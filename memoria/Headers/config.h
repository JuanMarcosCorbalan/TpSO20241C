#ifndef HEADERS_CONFIG_H_
#define HEADERS_CONFIG_H_

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/string.h>

typedef struct t_memoria_config {
	char* path_instrucciones;
	int puerto_escucha;
	int tam_memoria;
	int tam_pagina;
	int retardo_respuesta;
} t_memoria_config;

extern t_memoria_config* app_config;
extern t_config* archivo_config;

void cargar_config(char* path);
void finalizar_config(t_memoria_config* config);

#endif /* HEADERS_CONFIG_H_ */
