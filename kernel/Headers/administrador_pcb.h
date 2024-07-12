#ifndef HEADERS_ADMINISTRADOR_PCB_H_
#define HEADERS_ADMINISTRADOR_PCB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include "../../utils/Headers/models.h"
#include "../../utils/Headers/enums.h"
#include "log.h"
#include "config.h"

extern int contador_pid;
extern t_kernel_config* app_config;
extern t_list* lista_new;
extern t_list* lista_ready;
extern t_list* lista_v_ready;
extern t_list* lista_exec;
extern t_list* lista_blocked;
extern t_list* lista_exit;
extern t_list* lista_global;
extern sem_t sem_lista_new;
extern sem_t sem_lista_ready;
extern sem_t sem_lista_exec;
extern sem_t sem_lista_blocked;
extern pthread_mutex_t mutex_lista_new;
extern pthread_mutex_t mutex_lista_ready;
extern pthread_mutex_t mutex_lista_v_ready;
extern pthread_mutex_t mutex_lista_exec;
extern pthread_mutex_t mutex_lista_blocked;
extern pthread_mutex_t mutex_lista_exit;
extern pthread_mutex_t mutex_lista_global;

void finalizar_procesos();
t_pcb* crear_pcb(char* path_instrucciones);
t_pcb* obtener_pcb_por_id(int pid);
void remover_pcb(t_pcb* proceso, uint8_t estado);
void agregar_pcb(t_pcb* proceso, uint8_t estado);

#endif /* HEADERS_ADMINISTRADOR_PCB_H_ */
