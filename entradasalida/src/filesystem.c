#include "../Headers/filesystem.h"

void iniciar_filesystem(int block_size, int block_count){


	size_t tamanio_fs = block_count; //cantidad de bloques
	void* puntero_bitmap = malloc(tamanio_fs);
	bitarray = bitarray_create_with_mode(puntero_bitmap, tamanio_fs, LSB_FIRST);


	lista_metadata = list_create();
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

	agregar_a_lista_metadata(nombre, primer_bloque);
}

void delete(char* nombre){
	t_config* metadata = leer_metadata(nombre);
	int bloque_inicial = config_get_int_value(metadata, "BLOQUE_INICIAL");
	int tamanio = config_get_int_value(metadata, "TAMANIO_ARCHIVO");

	int cantidad_bloques_archivo = (tamanio/app_config->block_size);
	//int bloque_final = bloque_inicial + cantidad_bloques_archivo;

	desocupar_bloques_bitmap(cantidad_bloques_archivo, bloque_inicial);

	borrar_metadata_lista(nombre);

	config_destroy(metadata);
	remove(nombre);
}

void truncar(char* nombre, int nuevo_tamanio){
	t_metadata* metadata = buscar_metadata_lista(nombre);

	int bloque_final = metadata->tamanio/app_config->block_size;
	int nuevo_bloque_final = nuevo_tamanio/app_config->block_size;


	if(nuevo_tamanio > metadata->tamanio){
		extender_tamanio_archivo(nuevo_bloque_final - bloque_final, bloque_final);
	} else {
		desocupar_bloques_bitmap(bloque_final - nuevo_bloque_final, nuevo_bloque_final);
	}
	// actualizar metadata lista
	actualizar_metadata_lista(metadata);
	// actualizar .config

}

void extender_tamanio_archivo(int cant_bloques, int bloque_inicial){
//	if(!hay_bloques_contiguos_disponibles()){
//		compactacion();
//	}
//
//	ocupar_bloques_bitmap(cant_bloques, bloque_inicial);
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

void agregar_a_lista_metadata(char* nombre, int primer_bloque) {

	t_metadata* nuevo_metadata;
	strcpy(nuevo_metadata->nombre, nombre);
	nuevo_metadata->bloque_inicial = primer_bloque;
	nuevo_metadata->bloque_final = primer_bloque;
	nuevo_metadata->tamanio = 0;


	list_add(lista_metadata, nuevo_metadata);
}

t_metadata* buscar_metadata_lista(char* nombre){

	bool encontrar_por_nombre(void* elem){
		t_metadata* aux_metadata = (t_metadata*) elem;
		return(!strcmp(aux_metadata->nombre,nombre));
	}

	t_metadata*	metadata = list_find(lista_metadata, encontrar_por_nombre);
	return metadata;
}

void borrar_metadata_lista(char* nombre){

	bool encontrar_por_nombre(void* elem){
		t_metadata* aux_metadata = (t_metadata*) elem;
		return(!strcmp(aux_metadata->nombre,nombre));
	}
	void eliminar_metadata(void* elem){
		t_metadata* metadata = (t_metadata*) elem;
		list_remove_element(lista_metadata, metadata);
		free(metadata->nombre);
		free(metadata);
	}
	list_remove_and_destroy_by_condition(lista_metadata, encontrar_por_nombre,eliminar_metadata);
}

void actualizar_metadata_struct(t_metadata* metadata){

}
void actualizar_metadata_lista(t_metadata* metadata){

}
//void encontrar_nombre(char* nombre){
//	bool encontrar_por_nombre(void* elem){
//		t_metadata* aux_metadata = (t_metadata*) elem;
//		return(!strcmp(aux_metadata->nombre,nombre));
//	}
//
//}


