#include "../Headers/filesystem.h"

void iniciar_filesystem(int block_size, int block_count){


	size_t tamanio_fs = block_count; //cantidad de blockes
	void* puntero_bitmap = malloc(tamanio_fs);
	bitarray = bitarray_create_with_mode(puntero_bitmap, tamanio_fs, LSB_FIRST);

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

	int primer_bloque = buscar_primer_bloque_bitmap_libre();
	ocupar_bloques_bitmap(1, primer_bloque);

	crear_metadata(nombre, primer_bloque);

}

void delete(char* nombre){
	t_config* metadata = leer_metadata(nombre);
	int bloque_inicial = config_get_int_value(metadata, "BLOQUE_INICIAL");
	int tamanio = config_get_int_value(metadata, "TAMANIO_ARCHIVO");

	int cantidad_bloques_archivo = (tamanio/app_config->block_size);

	desocupar_bloques_bitmap(cantidad_bloques_archivo, bloque_inicial);

	config_destroy(metadata);
	remove(nombre);
}

void truncar(char* nombre, int nuevo_tamanio){
	t_config* metadata = leer_metadata(nombre);
	int bloque_inicial = config_get_int_value(metadata, "BLOQUE_INICIAL");
	int tamanio = config_get_int_value(metadata, "TAMANIO_ARCHIVO");

	int bloque_final = tamanio/app_config->block_size;
	int nuevo_bloque_final = nuevo_tamanio/app_config->block_size;


	if(nuevo_tamanio > tamanio){
		extender_tamanio_archivo(nuevo_bloque_final - bloque_final, bloque_final);
	} else {
		desocupar_bloques_bitmap(bloque_final - nuevo_bloque_final, nuevo_bloque_final);
	}
}

void extender_tamanio_archivo(int cant_bloques, int bloque_inicial){
//	if(!hay_bloques_contiguos_disponibles()){
//		compactacion();
//	}

	//ocupar_bloques_bitmap(bitarray, cant_bloques, bloque inicial);
}

int buscar_primer_bloque_bitmap_libre(){
	int bloque = 0;
	while(bitarray_test_bit(bitarray, bloque)){
		bloque++;
	}

	return bloque;
}

void ocupar_bloques_bitmap(int cant_bloques, int bloque_inicial){
	for(int i=0; i < cant_bloques; i++){
		bitarray_set_bit(bitarray, bloque_inicial + i);
	}
}

void desocupar_bloques_bitmap(int cant_bloques, int bloque_inicial){
	for(int i=0; i < cant_bloques; i++){
		bitarray_clean_bit(bitarray, bloque_inicial + i);
	}
}

void crear_metadata(char* nombre, int primer_bloque){
	FILE* archivo_metadata = fopen(nombre, "w");
	fclose(archivo_metadata);

	t_config *metadata = config_create(nombre);

	config_set_value(metadata, "BLOQUE_INICIAL", primer_bloque);
	config_set_value(metadata, "TAMANIO_ARCHIVO", "0");

	config_save_in_file(metadata, nombre);
	config_destroy(metadata);
}

t_config* leer_metadata(char* nombre){
	t_config *metadata = config_create(nombre);
	return metadata;
}

