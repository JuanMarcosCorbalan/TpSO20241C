#ifndef HEADERS_FILESYSTEM_H_
#define HEADERS_FILESYSTEM_H_

#include <commons/bitarray.h>
#include <commons/collections/queue.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"


//extern t_log* app_log;
extern t_io_config* app_config;
extern t_bitarray* bitarray;
//extern sem_t sem_conexiones;
//extern int socket_kernel;
//extern int socket_memoria;


/*
void iniciar_filesystem();

void truncate();

void validacion_espacios_contiguos_vacios();

void compactar();

void guardar_datos_de_archivo();

void realocar_archivo_en_bitmap();

void realocar_archivo_en_bloques();

void modificar_bloque_inicial_metadata();


*/

typedef struct t_metadata {
	char* nombre;
	int tamanio;
	int bloque_inicial;
	int bloque_final;
} t_metadata;


void create(char* nombre);

void delete(char* nombre);

void truncar(char* nombre, int nuevo_tamanio);

void extender_tamanio_archivo(int cant_bloques, int bloque_inicial);

int buscar_primer_bloque_bitmap_libre();

void ocupar_bloques_bitmap(int cant_bloques, int bloque_inicial);

void desocupar_bloques_bitmap(int cant_bloques, int bloque_inicial);

void crear_metadata(char* nombre, int primer_bloque);

t_config* leer_metadata(char* nombre);

#endif /* HEADERS_FILESYSTEM_H_ */
