#ifndef HEADERS_PLANIFICADOR_CORTO_PLAZO_H_
#define HEADERS_PLANIFICADOR_CORTO_PLAZO_H_

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "administrador_pcb.h"
#include "config.h"
#include "administrador_io.h"
#include "administrador_recurso.h"
#include "../../utils/Headers/enums.h"
#include "../../utils/Headers/mensajeria_kernel_memoria.h"
#include "../../utils/Headers/mensajeria_kernel_cpu.h"

extern t_kernel_config* app_config;
extern int socket_cpu_dispatch;
extern int socket_cpu_interrupt;
extern int socket_memoria;
extern int planificacion_pausada;
extern sem_t sem_planificacion_pausada;
extern sem_t sem_lista_ready;
extern sem_t sem_lista_exec;
extern sem_t sem_grado_multiprogramacion;
extern t_list* lista_ready;
extern t_list* lista_v_ready;
extern t_list* lista_exec;
extern pthread_mutex_t mutex_ejecucion_proceso;

void iniciar_planificador_corto_plazo();

#endif /* HEADERS_PLANIFICADOR_CORTO_PLAZO_H_ */
