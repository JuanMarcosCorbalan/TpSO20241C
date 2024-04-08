#ifndef HEADERS_CONFIG_H_
#define HEADERS_CONFIG_H_

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/string.h>

typedef struct t_cpu_config {
	char* ip_memoria;
	char* algoritmo_tlb;
	int puerto_memoria;
	int puerto_escucha_dispatch;
	int puerto_escucha_interrupt;
	int cantidad_entradas_tlb;
} t_cpu_config;

t_cpu_config* cargar_config(char* path);

#endif /* HEADERS_CONFIG_H_ */
