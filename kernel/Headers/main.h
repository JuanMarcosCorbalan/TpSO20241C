#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include <commons/collections/list.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"
#include "consola.h"
#include "startup.h"
#include "planificador_corto_plazo.h"
#include "planificador_largo_plazo.h"

t_kernel_config* app_config;
t_log* app_log;
t_config* app_config_file;

int socket_cpu_dispatch;
int socket_cpu_interrupt;
int socket_memoria;
int socket_escucha_io;
sem_t sem_conexiones;

t_list* lista_new;
t_list* lista_ready;
t_list* lista_v_ready;
t_list* lista_exec;
t_list* lista_blocked;
t_list* lista_exit;
t_list* lista_global;
sem_t sem_lista_new;
sem_t sem_lista_ready;
sem_t sem_lista_exec;
sem_t sem_lista_blocked;
pthread_mutex_t mutex_lista_new;
pthread_mutex_t mutex_lista_ready;
pthread_mutex_t mutex_lista_v_ready;
pthread_mutex_t mutex_lista_exec;
pthread_mutex_t mutex_lista_blocked;
pthread_mutex_t mutex_lista_exit;
pthread_mutex_t mutex_lista_global;
pthread_mutex_t mutex_lista_interfaz;
pthread_mutex_t mutex_planificador_largo_plazo;

int contador_pid;
int planificacion_pausada;
pthread_mutex_t mutex_contador_pid;
sem_t sem_grado_multiprogramacion;
sem_t sem_planificacion_pausada;
pthread_mutex_t mutex_ejecucion_proceso;

t_list* lista_interfaces;

#endif /* HEADERS_MAIN_H_ */
