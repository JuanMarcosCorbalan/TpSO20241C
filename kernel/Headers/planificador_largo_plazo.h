#ifndef HEADERS_PLANIFICADOR_LARGO_PLAZO_H_
#define HEADERS_PLANIFICADOR_LARGO_PLAZO_H_

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include "administrador_pcb.h"
#include "administrador_recurso.h"
#include "../../utils/Headers/mensajeria_kernel_memoria.h"
#include "../../utils/Headers/mensajeria_kernel_cpu.h"

extern int socket_memoria;
extern int socket_cpu_interrupt;
extern int socket_cpu_dispatch;
extern int planificacion_pausada;
extern sem_t sem_planificacion_pausada;
extern sem_t sem_lista_new;
extern sem_t sem_lista_ready;
extern sem_t sem_grado_multiprogramacion;
extern t_list* lista_new;
extern t_list* lista_ready;
extern pthread_mutex_t mutex_planificador_largo_plazo;

void iniciar_planificador_largo_plazo();
void finalizar_por_consola(uint32_t pid);

#endif /* HEADERS_PLANIFICADOR_LARGO_PLAZO_H_ */
