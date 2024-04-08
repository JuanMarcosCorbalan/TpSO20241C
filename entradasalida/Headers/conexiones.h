#ifndef HEADERS_CONEXIONES_H_
#define HEADERS_CONEXIONES_H_

#include "../../utils/Headers/connection.h"
#include "../../utils/Headers/communication.h"
#include <commons/log.h>
#include <pthread.h>
#include <semaphore.h>
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

extern t_log* app_log;
extern t_io_config* app_config;
extern sem_t sem_conexiones;
extern int socket_kernel;
extern int socket_memoria;

void iniciar_conexiones();

#endif /* HEADERS_CONEXIONES_H_ */
