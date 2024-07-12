#ifndef HEADERS_FILESYSTEM_H_
#define HEADERS_FILESYSTEM_H_

#include <commons/bitarray.h>
#include <commons/collections/queue.h>
#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "config.h"
#include <dirent.h>
#include <sys/mman.h>


extern t_log* app_log;
extern t_io_config* app_config;
extern t_bitarray* bitarray;
extern t_list* lista_metadata;

extern char* path_metadata;
extern char* path_bitarray;
extern char* path_bloques;
extern t_bitarray* bitarray_mem;
extern size_t tamanio_bitarray;

//extern sem_t sem_conexiones;
//extern int socket_kernel;
//extern int socket_memoria;


void iniciar_filesystem();

typedef struct t_metadata {
	char* nombre;
	int tamanio;
	int bloque_inicial;
	int bloque_final;
	int tamanio_real;
} t_metadata;


void create(int pid, char* nombre);

void delete(int pid, char* nombre);

void truncar(int pid, char* nombre, int nuevo_tamanio);

int extender_tamanio_archivo(int pid, t_metadata* metadata, int bloque_final, int *nuevo_bloque_final, int nuevo_tamanio);

int buscar_primer_bloque_bitmap_libre();

void ocupar_bloques_bitmap(int bloque_inicial, int bloque_final);

void desocupar_bloques_bitmap(int bloque_inicial, int bloque_final);

void crear_metadata(char* nombre, int primer_bloque);

t_config* leer_metadata(char* nombre);

void agregar_a_lista_metadata(char* nombre, int primer_bloque, int ultimo_bloque, int tamanio, int tamanio_real);

t_metadata* buscar_metadata_lista_por_nombre(char* nombre);

t_metadata* buscar_metadata_lista_por_bloque_inicial(int bloque_inicial);

void borrar_metadata_lista(char* nombre);

void actualizar_metadata(t_metadata* metadata, int nuevo_bloque_inicial, int nuevo_bloque_final, int nuevo_tamanio, int tamanio_real);

int buscar_primer_archivo_desde(int bloque);

void mover_archivo_en_bitmap();
void mover_archivo_en_binario();
void* copiar_y_remover(t_metadata* metadata);
int pegar_y_reubicar(t_metadata* metadata,void* info_binario, int bloque);

t_metadata* extraer_de_lista(t_metadata* metadata);

void compactacion(int pid, char* nombre_metadata_a_truncar);

void* leer_bloques(int tamanio_a_leer, int bloque_inicial);
void escribir_bloques(int tamanio_a_escribir, int bloque_inicial, int bloque_final, void* info_binario);

void write_fs(int pid, char* nombre, int tamanio, int puntero, void* info_a_escribir);
void* read_fs(int pid, char* nombre, int tamanio, int puntero);


char* crear_path_dat(char* nombre_dat);
char* crear_path_metadata(char* nombre_metadata);

void iniciar_bitmap(size_t tamanio_bitarray);

void leer_archivos_existentes();

void finalizar_estructuras();

#endif /* HEADERS_FILESYSTEM_H_ */
