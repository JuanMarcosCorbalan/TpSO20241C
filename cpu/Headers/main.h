#ifndef HEADERS_MAIN_H_
#define HEADERS_MAIN_H_

#include <stdlib.h>
#include <stdint.h>
#include "config.h"
#include "log.h"
#include "conexiones.h"
#include <commons/collections/list.h>

t_cpu_config* app_config;
t_log* app_log;
int socket_memoria;
int socket_escucha_interrupt;
int socket_escucha_dispatch;
int tamanio_pagina;
sem_t sem_conexiones;
int existe_interrupcion;
uint8_t motivo_interrupt_bloqueo;
uint8_t motivo_interrupt_exit;
t_list* lista_tlb;
t_temporal* rafaga_quantum;
t_config* archivo_config;

#endif /* HEADERS_MAIN_H_ */
