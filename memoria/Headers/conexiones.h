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
#include <unistd.h>

extern t_log* app_log;
extern t_memoria_config* app_config;

void iniciar_servidor();

#endif /* HEADERS_CONEXIONES_H_ */
