#include "../Headers/filesystem.h"

void iniciar_filesystem(){
	FILE* archivo_bloques ;

	tamanio_bitarray = app_config->block_count/8; //bits/8 -> tamanio en bytes

	path_bitarray = crear_path_bitarray();

	// abro el archivo de bloques o lo crea si no existe
	path_bloques = crear_path_bloques();

	archivo_bloques = fopen(path_bloques, "rb");

	if(archivo_bloques==NULL) {
		archivo_bloques = fopen(path_bloques, "wb+");
		char* bloque[app_config->block_size];
		memset(bloque, 0, app_config->block_size);
		for(int i = 0; i<app_config->block_count; i++){
			fwrite(bloque, 1, app_config->block_size, archivo_bloques);
		}
	}

	lista_metadata = list_create();
	// busca fcbs existentes
	leer_archivos_existentes();

	iniciar_bitmap(tamanio_bitarray);

	fclose(archivo_bloques);
}

void iniciar_bitmap(size_t tamanio_bitarray){

	FILE* archivo_bitarray = fopen(path_bitarray, "rb+");
	void* puntero_bitmap;
	if(archivo_bitarray) {
		// el archivo existe, mapea el contenido
		puntero_bitmap = mmap(NULL, tamanio_bitarray, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(archivo_bitarray), 0);
		if (puntero_bitmap == MAP_FAILED) {
			perror("Error al mapear el archivo de bitarray");
			fclose(archivo_bitarray);
		return;
		}
		bitarray_mem = bitarray_create_with_mode(puntero_bitmap, tamanio_bitarray, LSB_FIRST);
	} else {
		// si el archivo es nuevo
		archivo_bitarray = fopen(path_bitarray, "wb+");
		fflush(archivo_bitarray);
		ftruncate(fileno(archivo_bitarray), tamanio_bitarray);
		puntero_bitmap = mmap(NULL, tamanio_bitarray, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(archivo_bitarray), 0);
		memset(puntero_bitmap, 0, tamanio_bitarray);
		bitarray_mem = bitarray_create_with_mode(puntero_bitmap, tamanio_bitarray, LSB_FIRST);
	}


	msync(bitarray_mem->bitarray, tamanio_bitarray, MS_SYNC);

	fclose(archivo_bitarray);
}

void leer_archivos_existentes(){
    DIR *d;
    struct dirent *dir;
    int bloque_inicial = 0;
    int bloque_final = 0;
    int tamanio = 0;

    char* path_metadatas = malloc(strlen(app_config->path_base_dialfs) + strlen("/fcbs"));
    strcpy(path_metadatas, app_config->path_base_dialfs);
    strcat(path_metadatas, "/fcbs");
    d = opendir(path_metadatas);

    if (d){
    	while((dir = readdir(d)) != NULL){
    		if(dir->d_type == DT_REG) {
    			char* path_config_a_cargar = malloc(strlen(path_metadatas) + strlen("/") + strlen(dir->d_name) + 1);
    			strcpy(path_config_a_cargar, path_metadatas);
    			strcat(path_config_a_cargar, "/");
    			strcat(path_config_a_cargar, dir->d_name);
    			//srcpy(metadata[count]->nombre, dir->d_name);
    			t_config* config_a_cargar = config_create(path_config_a_cargar);
    			bloque_inicial = config_get_int_value(config_a_cargar, "BLOQUE_INICIAL");
    			tamanio = config_get_int_value(config_a_cargar, "TAMANIO_ARCHIVO");
    			bloque_final = bloque_inicial + (tamanio/app_config->block_size) - 1;
    			agregar_a_lista_metadata(dir->d_name, bloque_inicial, bloque_final, tamanio);
    			free(path_config_a_cargar);
    		}

    	}
    	closedir(d);
    } else {
    	log_info(app_log, "No se pudo abrir el directorio");
    }
}
/*
 * IO_FS_CREATE (Interfaz, Nombre Archivo): Esta instrucción solicita al Kernel que mediante la interfaz seleccionada,
 * se cree un archivo en el FS montado en dicha interfaz.
 *
 */
char* crear_path_metadata(char* nombre_metadata){
	char* path_metadata_nuevo = malloc(strlen(app_config->path_base_dialfs) + strlen("/fcbs/") + strlen(nombre_metadata) + 1);
	strcpy(path_metadata_nuevo, app_config->path_base_dialfs);
	strcat(path_metadata_nuevo, "/fcbs/");
	strcat(path_metadata_nuevo, nombre_metadata);
	return path_metadata_nuevo;
}

char* crear_path_bitarray(){
	char* path_bitarray_nuevo = malloc(strlen(app_config->path_base_dialfs) + strlen("/") + strlen("bitarray.dat") + 1);
	strcpy(path_bitarray_nuevo, app_config->path_base_dialfs);
	strcat(path_bitarray_nuevo, "/");
	strcat(path_bitarray_nuevo, "bitarray.dat");
	return path_bitarray_nuevo;
}

char* crear_path_bloques(){
	char* path_bloques_nuevo = malloc(strlen(app_config->path_base_dialfs) + strlen("/") + strlen("bloques.dat") + 1);
	strcpy(path_bloques_nuevo, app_config->path_base_dialfs);
	strcat(path_bloques_nuevo, "/");
	strcat(path_bloques_nuevo, "bloques.dat");
	return(path_bloques_nuevo);
}
void create(int pid, char* nombre){

	int primer_bloque = buscar_primer_bloque_bitmap_libre();
	ocupar_bloques_bitmap(primer_bloque, primer_bloque);

	crear_metadata(nombre, primer_bloque);

	agregar_a_lista_metadata(nombre, primer_bloque, primer_bloque, 16);
	log_info(app_log, "PID: %d - Crear Archivo: %s", pid, nombre);
}

void delete(int pid, char* nombre){
	char* path = crear_path_metadata(nombre);
	t_config* metadata = leer_metadata(path);
	int bloque_inicial = config_get_int_value(metadata, "BLOQUE_INICIAL");
	int tamanio = config_get_int_value(metadata, "TAMANIO_ARCHIVO");
	int bloque_final;
	int cantidad_bloques_archivo = (tamanio/app_config->block_size);

	if(cantidad_bloques_archivo != 0) {
		bloque_final = bloque_inicial + cantidad_bloques_archivo - 1;
	} else {
		bloque_final = bloque_inicial;
	}
	desocupar_bloques_bitmap(bloque_inicial, bloque_final);

	borrar_metadata_lista(nombre);

	config_destroy(metadata);
	remove(path);
	log_info(app_log, "PID: %d - Eliminar Archivo: %s", pid, nombre);
}

void truncar(int pid, char* nombre, int nuevo_tamanio){
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre);

	int bloque_inicial = metadata->bloque_inicial;
	int bloque_final = metadata->bloque_final;

	int bloques_asignados_actual = metadata->tamanio/app_config->block_size;
	if(metadata->tamanio%app_config->block_size){
		bloques_asignados_actual += 1;
	}
	int cantidad_bloques_nueva = nuevo_tamanio/app_config->block_size;
	if(nuevo_tamanio%app_config->block_size){
		cantidad_bloques_nueva += 1;
	}
	int nuevo_bloque_final = bloque_inicial + cantidad_bloques_nueva - 1;



	if(cantidad_bloques_nueva > bloques_asignados_actual){
		nuevo_bloque_final = extender_tamanio_archivo(pid, metadata, bloque_final, &nuevo_bloque_final, nuevo_tamanio);
	} else if(cantidad_bloques_nueva < bloques_asignados_actual){
		desocupar_bloques_bitmap(nuevo_bloque_final + 1, bloque_final);
	}
	// actualizar metadata lista, actualizar .config
	if(nuevo_bloque_final < bloque_final){
		nuevo_bloque_final = 0;
	}
	actualizar_metadata(metadata, metadata->bloque_inicial, nuevo_bloque_final, cantidad_bloques_nueva * app_config->block_size);
	log_info(app_log, "PID: %d - Truncar Archivo: %s - Tamaño: %d ",pid , nombre, cantidad_bloques_nueva * app_config->block_size);

}

int extender_tamanio_archivo(int pid, t_metadata* metadata, int bloque_final, int* nuevo_bloque_final, int nuevo_tamanio){
	int nuevo_bloque_final_int = *nuevo_bloque_final;
	if(hay_bloques_contiguos_disponibles(bloque_final ,nuevo_bloque_final_int - bloque_final) < 0){
		compactacion(pid, metadata->nombre);
		nuevo_bloque_final_int = metadata->bloque_inicial + nuevo_tamanio/app_config->block_size -1;
		if(nuevo_tamanio%app_config->block_size){
			nuevo_bloque_final_int += 1;
		}
	}
	bloque_final = metadata->bloque_final;
	ocupar_bloques_bitmap(bloque_final+1, nuevo_bloque_final_int);
	return nuevo_bloque_final_int;
}

int buscar_primer_bloque_bitmap_libre(){
	int bloque = 0;
	while(bitarray_test_bit(bitarray_mem, bloque)){
		bloque++;
	}

	return bloque;
}

void ocupar_bloques_bitmap(int bloque_inicial, int bloque_final){
	for(int i=bloque_inicial; i <=bloque_final; i++){
		bitarray_set_bit(bitarray_mem, i);
		msync(bitarray_mem->bitarray,tamanio_bitarray, MS_SYNC);
	}
}

void desocupar_bloques_bitmap(int bloque_inicial, int bloque_final){
	for(int i=bloque_inicial; i <= bloque_final; i++){
		bitarray_clean_bit(bitarray_mem, i);
		msync(bitarray_mem, tamanio_bitarray, MS_SYNC);
	}
}

void crear_metadata(char* nombre, int primer_bloque){
	char* path = crear_path_metadata(nombre);
	FILE* archivo_metadata = fopen(path, "w");
	fclose(archivo_metadata);

	t_config *metadata = config_create(path);

	char primer_bloque_str[12];  // Suficientemente grande para contener el valor máximo de un uint32_t
	sprintf(primer_bloque_str, "%u", primer_bloque);
	config_set_value(metadata, "BLOQUE_INICIAL", primer_bloque_str);
	config_set_value(metadata, "TAMANIO_ARCHIVO", "16");

	config_save_in_file(metadata, path);
	config_destroy(metadata);
}

t_config* leer_metadata(char* path){
	t_config* metadata = config_create(path);
	return metadata;
}

void agregar_a_lista_metadata(char* nombre, int primer_bloque, int bloque_final, int tamanio) {

	t_metadata* nuevo_metadata = malloc(sizeof(t_metadata));
	nuevo_metadata->nombre = malloc(strlen(nombre) + 1);
	strcpy(nuevo_metadata->nombre, nombre);
	nuevo_metadata->bloque_inicial = primer_bloque;
	nuevo_metadata->bloque_final = bloque_final;
	nuevo_metadata->tamanio = tamanio;

	list_add(lista_metadata, nuevo_metadata);
}

t_metadata* buscar_metadata_lista_por_nombre(char* nombre){

	bool encontrar_por_nombre(void* elem){
		t_metadata* aux_metadata = (t_metadata*) elem;
		return(strcmp(aux_metadata->nombre,nombre) == 0);
	}

	t_metadata*	metadata = list_find(lista_metadata, encontrar_por_nombre);
	return metadata;
}

void borrar_metadata_lista(char* nombre){
	bool encontrar_por_nombre(void* elem){
		t_metadata* aux_metadata = (t_metadata*) elem;
		return(strcmp(aux_metadata->nombre,nombre) == 0);
	}
	void eliminar_metadata(void* elem){
		t_metadata* metadata = (t_metadata*) elem;
		list_remove_element(lista_metadata, metadata);
		free(metadata->nombre);
		free(metadata);
	}
	list_remove_and_destroy_by_condition(lista_metadata,encontrar_por_nombre,eliminar_metadata);
}

void actualizar_metadata(t_metadata* metadata, int nuevo_bloque_inicial, int nuevo_bloque_final, int nuevo_tamanio){
	metadata->bloque_inicial = nuevo_bloque_inicial;
	metadata->bloque_final = nuevo_bloque_final;
	metadata->tamanio = nuevo_tamanio;
	char* nombre_metadata = malloc(strlen(metadata->nombre)+1);
	memcpy(nombre_metadata, metadata->nombre, strlen(metadata->nombre)+1);
	char* path = crear_path_metadata(nombre_metadata);

	t_config *metadata_config = config_create(path);


	// hay que pasar el int a string
	char primer_bloque_str[12];  // Suficientemente grande para contener el valor máximo de un uint32_t
	sprintf(primer_bloque_str, "%u", nuevo_bloque_inicial);
	char nuevo_tamanio_str[12];
	sprintf(nuevo_tamanio_str, "%u", nuevo_tamanio);
	config_set_value(metadata_config, "TAMANIO_ARCHIVO", nuevo_tamanio_str);
	config_set_value(metadata_config, "BLOQUE_INICIAL", primer_bloque_str);

	config_save(metadata_config);
	config_destroy(metadata_config);
	free(path);
//	free(metadata_config);
}


int buscar_primer_archivo_desde(int bloque){
	size_t tamanio_bitmap = bitarray_get_max_bit(bitarray_mem);
	while((bloque < tamanio_bitmap) && !bitarray_test_bit(bitarray_mem, bloque)){
		bloque++;
	}
	if (bloque == tamanio_bitmap){
		return -1;
	}
	return bloque;
}
t_metadata* buscar_metadata_lista_por_bloque_inicial(int bloque_inicial){

	bool encontrar_por_bloque_inicial(void* elem){
		t_metadata* aux_metadata = (t_metadata*) elem;
		return(aux_metadata->bloque_inicial == bloque_inicial);
	}

	t_metadata*	metadata = list_find(lista_metadata, encontrar_por_bloque_inicial);
	return metadata;
}

void* copiar_y_remover(t_metadata* metadata){
	void* buffer_leido = leer_bloques(metadata->tamanio, metadata->bloque_inicial);
	//desocupa bitmap
	desocupar_bloques_bitmap(metadata->bloque_inicial,metadata->bloque_final);

	return buffer_leido;
}

int pegar_y_reubicar(t_metadata* metadata, void* info_binario, int primer_bloque_libre){
	// ocupa bitmap
	int cantidad_bloques = metadata->tamanio/app_config->block_size;
	int ultimo_bloque_a_ocupar = primer_bloque_libre + cantidad_bloques - 1;
	ocupar_bloques_bitmap(primer_bloque_libre,ultimo_bloque_a_ocupar);
	// pega la info en el binario (ocupa bloques)
	// devuelve el ultimo bloque usado
	escribir_bloques(metadata->tamanio, primer_bloque_libre,metadata->bloque_final,info_binario);
	return ultimo_bloque_a_ocupar;
}
void escribir_bloques(int tamanio, int bloque_inicial, int bloque_final, void* info_binario){
	FILE* archivo_bloques = fopen(path_bloques, "rb+");
	fseek(archivo_bloques, bloque_inicial * app_config->block_size, SEEK_SET);
	fwrite(info_binario, tamanio, 1, archivo_bloques);
}
void* leer_bloques(int tamanio, int bloque_inicial){
	void* buffer_leido = malloc(tamanio);
	FILE* archivo_bloques = fopen(path_bloques, "rb+");
	fseek(archivo_bloques, bloque_inicial * app_config->block_size, SEEK_SET);
	fread(buffer_leido, tamanio, 1, archivo_bloques);
	fclose(archivo_bloques);
	return buffer_leido;
}

int hay_bloques_contiguos_disponibles(int ultimo_bloque_archivo, int cant_bloques){
    int bitarray_length = bitarray_get_max_bit(bitarray_mem);
    int pos = ultimo_bloque_archivo + 1;
    int bloque_libre_inicial = 0;
    int bloques_libres_contiguos = 0;

    while(pos < bitarray_length && bloques_libres_contiguos < cant_bloques){
    	bloques_libres_contiguos = 0;
        bloque_libre_inicial = pos;

        if(bitarray_test_bit(bitarray_mem, pos)){
        	return -1;
        } else {
        	while(!bitarray_test_bit(bitarray_mem, pos)){
        		bloques_libres_contiguos ++;
        		pos ++;
        	}

        }
    }
    if(bloques_libres_contiguos >= cant_bloques){
    	return bloque_libre_inicial;
    } else {
    	return -1;
    }


}


void compactacion(int pid, char* nombre_metadata_a_truncar){
	log_info(app_log, "PID: %d - Inicio Compactación", 1);
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre_metadata_a_truncar);
	void* buffer_archivo_truncado = copiar_y_remover(metadata);
	t_metadata* metadata_aux;
	void* info;
	int primer_bloque_libre = buscar_primer_bloque_bitmap_libre();
	int primer_bloque_archivo = buscar_primer_archivo_desde(primer_bloque_libre);
	//int bloque_final;
	int ultimo_bloque_usado;

	while(primer_bloque_archivo >= 0){
		// hay que buscar el primer archivo a ubicar
		// resolver logica de como arrancar
		metadata_aux = buscar_metadata_lista_por_bloque_inicial(primer_bloque_archivo);
		info = copiar_y_remover(metadata_aux);

		ultimo_bloque_usado = pegar_y_reubicar(metadata_aux, info, primer_bloque_libre);

		actualizar_metadata(metadata_aux, primer_bloque_libre, ultimo_bloque_usado, metadata_aux->tamanio);

		primer_bloque_libre = ultimo_bloque_usado + 1;
		// esto es para ir con el siguiente, devuelve -1 si ya no hay archivos siguientes (esta todo compactado)
		primer_bloque_archivo = buscar_primer_archivo_desde(primer_bloque_libre);
	}
	ultimo_bloque_usado = pegar_y_reubicar(metadata, buffer_archivo_truncado, primer_bloque_libre);
	actualizar_metadata(metadata, primer_bloque_libre, ultimo_bloque_usado, metadata->tamanio);
	log_info(app_log, "PID: %d - Fin Compactación", 1);
	//sleep(app_config->retraso_compactacion/100);
	//free(metadata_aux);
}

// aca reescribo leer bloques y escribir bloques pero pasandole el puntero por parametro
void write_fs(int pid, char* nombre, int tamanio, int puntero, void* info_a_escribir){
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre);
	FILE* archivo_bloques = fopen(path_bloques, "wb+");
	// el puntero es relativo del archivo.
	fseek(archivo_bloques, (metadata->bloque_inicial * app_config->block_size) + puntero, SEEK_SET);
	fwrite(info_a_escribir, tamanio, 1, archivo_bloques);
	fclose(archivo_bloques);
	log_info(app_log, "PID: %d - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", pid, nombre, tamanio, puntero);
}

void* read_fs(int pid, char* nombre, int tamanio, int puntero){
	void* info_leida = malloc(tamanio);
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre);
	FILE* archivo_bloques = fopen(path_bloques, "rb+");
	// el puntero es relativo del archivo.
	fseek(archivo_bloques, (metadata->bloque_inicial * app_config->block_size) + puntero, SEEK_SET);
	fread(info_leida, tamanio, 1, archivo_bloques);
	fclose(archivo_bloques);
	log_info(app_log, "PID: %d - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d", pid, nombre, tamanio, puntero);
	free(metadata);
	return info_leida;
}

