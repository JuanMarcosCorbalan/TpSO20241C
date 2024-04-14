#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"
#include "interfaces.h"

t_io_config* app_config;
t_log* app_log;
sem_t sem_conexiones;
int socket_kernel;
int socket_memoria;

#endif /* HEADERS_MAIN_H_ */
