#ifndef HEADERS_CONEXIONES_H_
#define HEADERS_CONEXIONES_H_

#include "../../utils/Headers/connection.h"
#include "../../utils/Headers/communication.h"
#include "administrador_io.h"
#include <commons/log.h>
#include <pthread.h>
#include <semaphore.h>
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern t_log* app_log;
extern int socket_cpu_dispatch;
extern int socket_cpu_interrupt;
extern int socket_memoria;
extern int socket_escucha_io;
extern t_kernel_config* app_config;
extern sem_t sem_conexiones;

void iniciar_conexiones();

#endif /* HEADERS_CONEXIONES_H_ */
