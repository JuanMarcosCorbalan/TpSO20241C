/*
 * administrador_memoria.h
 *
 *  Created on: Apr 5, 2024
 *      Author: utnso
 */

#ifndef HEADERS_ADMINISTRADOR_MEMORIA_H_
#define HEADERS_ADMINISTRADOR_MEMORIA_H_

#include <stdint.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include "config.h"

//Estructuras del proceso
typedef struct t_memoria_proceso
{
    uint32_t pid;
    int tamanio_asignado;
    int cantidad_de_paginas;
    int *tabla_de_paginas;
} t_memoria_proceso;
extern t_list* memoria_procesos;

//Memoria de usuario
extern void *memoria_de_usuario;
extern t_bitarray *tabla_de_frames;

void iniciar_memoria_de_usuario();
void cargar_memoria_de_proceso(uint32_t pid);

void asignar_frames(t_memoria_proceso*,int);
void liberar_frames(t_memoria_proceso*,int);
void cambiar_tamanio(t_memoria_proceso*,int);
void asignar_memoria(t_memoria_proceso*,int);

void liberar_memoria(t_memoria_proceso*,int);
t_memoria_proceso *buscar_proceso(int);

int escribir_en_memoria(t_memoria_proceso*,int, void*,int);
void *leer_en_memoria(t_memoria_proceso*,int,int);



#endif /* HEADERS_ADMINISTRADOR_MEMORIA_H_ */
