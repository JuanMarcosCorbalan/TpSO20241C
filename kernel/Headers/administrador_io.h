#ifndef HEADERS_ADMINISTRADOR_IO_H_
#define HEADERS_ADMINISTRADOR_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "administrador_pcb.h"
#include <commons/collections/list.h>
#include "../../utils/Headers/models.h"
#include "../../utils/Headers/communication.h"
#include "../../utils/Headers/mensajeria_kernel_io.h"

typedef struct t_interfaz_io {
	char* nombre;
	uint32_t estado_conexion;
	uint8_t estado_validacion;
	sem_t sem_bloqueo_global;
	sem_t sem_espera_global;
	int* socket_io;
	t_list* bloqueados;
} t_interfaz_io;

extern t_list* lista_interfaces;
extern pthread_mutex_t mutex_lista_interfaz;
extern sem_t sem_lista_ready;
extern sem_t sem_planificacion_pausada;
extern sem_t sem_grado_multiprogramacion;
extern int planificacion_pausada;

void operar_io(int* socket_io);
int validar_disponibilidad_interfaz(char* nombre);
t_interfaz_io* obtener_interfaz(char* nombre);

#endif /* HEADERS_ADMINISTRADOR_IO_H_ */
