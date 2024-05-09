#include "../Headers/filesystem.h"

void iniciar_filesystem(int block_size, int block_count){

	/*
	 * if(bloques_no_existe){
	 * 	FILE* bloques = fopen("bloques.dat", "w");
	 * }
	 *
	 * if(bitmap_no_existe){
	 * 	FILE* bitmap = fopen("bitmap.dat", "w");
	 * }
	 *
	 * fclose(bloques);
	 * fclose(bitmap);
	 */


}

/*
 * IO_FS_CREATE (Interfaz, Nombre Archivo): Esta instrucción solicita al Kernel que mediante la interfaz seleccionada,
 * se cree un archivo en el FS montado en dicha interfaz.
 *
 */

void create(char* nombre){

	int primer_bloque = buscar_primer_bloque_bitmap_libre(bitarray);
	ocupar_bloques_bitmap(bitarray, 1, primer_bloque);

	crear_metadata(nombre, primer_bloque);

}

void delete(char* nombre){
	t_config* metadata = leer_metadata(nombre);
	int bloque_inicial = config_getvalue(metadata, "BLOQUE_INICIAL");
	int tamanio = config_getvalue(metadata, "TAMANIO_ARCHIVO");

	int cantidad_bloques_archivo = tamanio/block_size;

	desocupar_bloques_bitmap(bitarray, cantidad_bloques_archivo, bloque_inicial);

	config_destroy(metadata);
	remove(nombre);
}

void truncate(char* nombre, int nuevo_tamanio){
	t_config* metadata = leer_metadata(nombre);
	int bloque_inicial = config_getvalue(metadata, "BLOQUE_INICIAL");
	int tamanio = config_getvalue(metadata, "TAMANIO_ARCHIVO");

	int bloque_final = tamanio/block_size;
	int nuevo_bloque_final = nuevo_tamanio/block_size;


	if(nuevo_tamanio > tamanio){
		extender_tamanio_archivo(bitarray, nuevo_bloque_final - bloque_final, bloque_final);
	} else {
		desocupar_bloques_bitmap(bitarray, bloque_final - nuevo_bloque_final, nuevo_bloque_final);
	}
}

void extender_tamanio_archivo(int cant_bloques, int bloque_inicial){
	if(!hay_bloques_contiguos_disponibles()){
		compactacion();
	}

	ocupar_bloques_bitmap(bitarray, cant_bloques, bloque inicial);
}

int buscar_primer_bloque_bitmap_libre(t_bitarray* bitarray){
	int bloque = 0;
	while(bitarray_test_bit(bitarray, bloque)){
		bloque++;
	}

	return bloque;
}

void ocupar_bloques_bitmap(t_bitarray* bitarray, int cant_bloques, int bloque_inicial){

	for(int i=0; i < cant_bloques; i++){
		bitarray_set_bit(bitarray, bloque_inicial + i);
	}

void desocupar_bloques_bitmap(t_bitarray* bitarray, int cant_bloques, int bloque_inicial){
	for(int i=0; i < cant_bloques; i++){
		bitarray_clean_bit(bitarray, bloque_inicial + i);
	}
}

void crear_metadata(char* nombre, int primer_bloque){
	FILE* archivo_metadata = fopen(nombre, "w");
	fclose(archivo_metadata);

	t_config *metadata = config_create(nombre));

	config_set_value(metadata, "BLOQUE_INICIAL", primer_bloque);
	config_set_value(metadata, "TAMANIO_ARCHIVO", "0");

	config_save_in_file(metadata, nombre);
	config_destroy(metadata);
}

t_config* leer_metadata(char* nombre){
	t_config *metadata = config_create(nombre));
	return metadata;
}

