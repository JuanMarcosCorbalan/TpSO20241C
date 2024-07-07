#ifndef HEADERS_CONEXIONES_H_
#define HEADERS_CONEXIONES_H_

#include "../../utils/Headers/connection.h"
#include "../../utils/Headers/communication.h"
#include "../../utils/Headers/mensajeria_kernel_memoria.h"
#include "../../utils/Headers/mensajeria_cpu_memoria.h"
#include "../../utils/Headers/mensajeria_io_memoria.h"
#include "../../utils/Headers/models.h"
#include "../../utils/Headers/enums.h"
#include <commons/log.h>
#include <pthread.h>
#include <semaphore.h>
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include "administrador_instrucciones.h"
#include "administrador_memoria.h"
#include <unistd.h>

extern t_log* app_log;
extern t_memoria_config* app_config;
extern t_list* instrucciones_procesos;
extern pthread_mutex_t mutex_acceso_memoria;

void iniciar_servidor();

#endif /* HEADERS_CONEXIONES_H_ */
