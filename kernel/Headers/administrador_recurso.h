#ifndef HEADERS_ADMINISTRADOR_RECURSO_H_
#define HEADERS_ADMINISTRADOR_RECURSO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include "../../utils/Headers/models.h"
#include "../../utils/Headers/enums.h"
#include "log.h"
#include "administrador_pcb.h"
#include "config.h"

extern t_kernel_config* app_config;
extern t_list* lista_ready;
extern t_list* lista_blocked;
extern sem_t sem_lista_ready;

int existe_recurso(char* nombre);
int operar_wait(t_pcb* proceso, char* nombre);
void operar_signal(t_pcb* proceso, char* nombre);
void desbloquear_proceso_recurso(char* nombre);
void liberar_recursos(t_pcb* proceso);

#endif /* HEADERS_ADMINISTRADOR_RECURSO_H_ */
