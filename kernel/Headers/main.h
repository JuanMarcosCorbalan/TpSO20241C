#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"

t_kernel_config* app_config;
t_log* app_log;

int socket_cpu_dispatch;
int socket_cpu_interrupt;
int socket_memoria;
int socket_escucha_io;

sem_t sem_conexiones;

#endif /* HEADERS_MAIN_H_ */
