#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include "administrador_memoria.h"

t_list* instrucciones_procesos;
t_memoria_config* app_config;
t_log* app_log;
t_list* tabla_paginas;
void* espacio_memoria;
void* bits_memoria;
t_bitarray* bitarray_memoria;

#endif /* HEADERS_MAIN_H_ */
