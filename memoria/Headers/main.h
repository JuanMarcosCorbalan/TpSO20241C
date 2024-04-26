#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>
#include <commons/bitarray.h>

t_list* instrucciones_procesos;
t_memoria_config* app_config;
t_log* app_log;

t_list* memoria_procesos;

//Memoria de usuario
void *memoria_de_usuario;
t_bitarray *tabla_de_frames;

#endif /* HEADERS_MAIN_H_ */
