#ifndef HEADERS_CONSOLA_H_
#define HEADERS_CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <readline/readline.h>
#include <commons/string.h>
#include "administrador_pcb.h"
#include "log.h"
#include "administrador_io.h"
#include "planificador_largo_plazo.h"

typedef enum t_operacion_consola {
	OPERACION_INCORRECTA,
	EJECUTAR_SCRIPT,
	INICIAR_PROCESO,
	MULTIPROGRAMACION,
	FINALIZAR_PROCESO,
	INICIAR_PLANIFICACION,
	DETENER_PLANIFICACION,
	PROCESO_ESTADO,
	FINALIZAR
} t_operacion_consola;

extern t_list* lista_new;
extern t_list* lista_ready;
extern t_list* lista_global;
extern t_list* lista_interfaces;
extern sem_t sem_lista_new;
extern sem_t sem_lista_ready;
extern pthread_mutex_t mutex_lista_new;
extern pthread_mutex_t mutex_lista_ready;
extern pthread_mutex_t mutex_lista_global;
extern pthread_mutex_t mutex_contador_pid;

extern int planificacion_pausada;
extern sem_t sem_planificacion_pausada;

void iniciar_consola();

#endif /* HEADERS_CONSOLA_H_ */
