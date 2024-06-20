#ifndef HEADERS_CICLO_INSTRUCCION_H_
#define HEADERS_CICLO_INSTRUCCION_H_

#include "../../utils/Headers/connection.h"
#include "../../utils/Headers/communication.h"
#include "../../utils/Headers/mensajeria_cpu_memoria.h"
#include "../../utils/Headers/mensajeria_kernel_cpu.h"
#include "log.h"
#include <commons/log.h>
#include <pthread.h>
#include <semaphore.h>
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>

extern t_log* app_log;
extern t_cpu_config* app_config;
extern int socket_memoria;
extern int socket_escucha_interrupt;
extern int socket_escucha_dispatch;
extern int existe_interrupcion;
extern int tamanio_pagina;
extern uint8_t motivo_interrupt_bloqueo;
extern uint8_t motivo_interrupt_exit;
extern t_list* lista_tlb;
extern t_temporal* rafaga_quantum;

void operar_dispatch(int* socket_cliente);
void ejecutar_proceso(dt_contexto_proceso* contexto_proceso, int socket_cliente);

#endif /* HEADERS_CICLO_INSTRUCCION_H_ */
