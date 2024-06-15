#ifndef HEADERS_LOG_H_
#define HEADERS_LOG_H_

#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include "config.h"

extern t_log* app_log;
extern t_kernel_config* app_config;
extern t_list* lista_ready;
extern t_list* lista_v_ready;

void iniciar_log(char* path);
void logear_creacion_proceso(int pid);
void logear_fin_proceso(int pid, char* motivo);
void logear_cambio_estado(int pid, char* estado_anterior, char* estado_actual);
void logear_motivo_bloqueo(int pid, char* motivo);
void logear_fin_quantum(int pid);
void logear_ingreso_ready();
void logear_ingreso_v_ready();

#endif /* HEADERS_LOG_H_ */
