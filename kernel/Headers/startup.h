#ifndef HEADERS_STARTUP_H_
#define HEADERS_STARTUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "config.h"
#include <commons/collections/list.h>

extern int contador_pid;
extern int planificacion_pausada;
extern t_kernel_config* app_config;
extern t_list* lista_new;
extern t_list* lista_ready;
extern t_list* lista_v_ready;
extern t_list* lista_exec;
extern t_list* lista_blocked;
extern t_list* lista_exit;
extern t_list* lista_global;
extern t_list* lista_interfaces;
extern sem_t sem_lista_new;
extern sem_t sem_lista_ready;
extern sem_t sem_lista_exec;
extern sem_t sem_lista_blocked;
extern sem_t sem_grado_multiprogramacion;
extern sem_t sem_planificacion_pausada;
extern pthread_mutex_t mutex_lista_new;
extern pthread_mutex_t mutex_lista_ready;
extern pthread_mutex_t mutex_lista_v_ready;
extern pthread_mutex_t mutex_lista_exec;
extern pthread_mutex_t mutex_lista_blocked;
extern pthread_mutex_t mutex_lista_exit;
extern pthread_mutex_t mutex_lista_global;
extern pthread_mutex_t mutex_contador_pid;
extern pthread_mutex_t mutex_ejecucion_proceso;
extern pthread_mutex_t mutex_lista_interfaz;
extern pthread_mutex_t mutex_planificador_largo_plazo;

void iniciar_estructuras();
void finalizar_estructuras();

#endif /* HEADERS_STARTUP_H_ */
