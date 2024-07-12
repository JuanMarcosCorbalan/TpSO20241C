#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include "config.h"
#include <math.h>
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include "administrador_memoria.h"

t_list* instrucciones_procesos;
t_memoria_config* app_config;
t_log* app_log;
t_list* lista_global_marcos;
t_list* lista_marcos_procesos;
void* espacio_memoria;
void* puntero_bitmap;
t_bitarray* bitarray_memoria;
pthread_mutex_t mutex_acceso_memoria;
t_config* archivo_config;

#endif /* HEADERS_MAIN_H_ */
