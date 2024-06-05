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
extern t_list* lista_metadata;

//extern sem_t sem_conexiones;
//extern int socket_kernel;
//extern int socket_memoria;

/*
void iniciar_filesystem();

void truncate();

void validacion_espacios_contiguos_vacios();



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

void extender_tamanio_archivo(t_metadata* metadata, int bloque_final, int *nuevo_bloque_final, int nuevo_tamanio);

int buscar_primer_bloque_bitmap_libre();

void ocupar_bloques_bitmap(int bloque_inicial, int bloque_final);

void desocupar_bloques_bitmap(int bloque_inicial, int bloque_final);

void crear_metadata(char* nombre, int primer_bloque);

t_config* leer_metadata(char* nombre);

void agregar_a_lista_metadata(char* nombre, int primer_bloque);

t_metadata* buscar_metadata_lista_por_nombre(char* nombre);

t_metadata* buscar_metadata_lista_por_bloque_inicial(int bloque_inicial);

void borrar_metadata_lista(char* nombre);

void actualizar_metadata(t_metadata* metadata, int nuevo_bloque_inicial, int nuevo_bloque_final, int nuevo_tamanio);

int buscar_primer_archivo_desde(int bloque);

void mover_archivo_en_bitmap();
void mover_archivo_en_binario();
t_list* copiar_y_remover(t_metadata* metadata);
int pegar_y_reubicar(t_metadata* metadata,t_list* info_binario, int bloque);

int hay_bloques_contiguos_disponibles(int cantidad_bloques);

t_metadata* extraer_de_lista(t_metadata* metadata);

void compactacion(t_metadata* metadata);

t_list* leer_bloques(int bloque_inicial, int bloque_final);

#endif /* HEADERS_FILESYSTEM_H_ */
