#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"
#include "interfaces.h"
#include <pthread.h>
#include "filesystem.h"

t_io_config* app_config;
t_config* archivo_config;
t_log* app_log;
sem_t sem_conexiones;
int socket_kernel;
int socket_memoria;
pthread_mutex_t mutex_kernel;
t_list* lista_metadata;
char* path_metadata;
char* path_bitarray;
char* path_bloques;
t_bitarray* bitarray_mem;
size_t tamanio_bitarray;
#endif /* HEADERS_MAIN_H_ */
