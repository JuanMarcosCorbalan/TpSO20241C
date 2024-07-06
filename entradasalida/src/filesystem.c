#include "../Headers/filesystem.h"

void iniciar_filesystem(int block_size, int block_count){

	tamanio_bitarray = block_count/8; //bits/8 -> tamanio en bytes

	path_bitarray = crear_path_bitarray();

	// abro el archivo de bloques o lo crea si no existe
	path_bloques = crear_path_bloques();

	FILE* archivo_bloques = fopen(path_bloques, "wb+");

	lista_metadata = list_create();
	leer_archivos_existentes();
	// aca tendria que ver si leo un archivo que tenga los elementos de la lista
	// si no estaba vacio el fs (seria lo correcto).

	iniciar_bitmap(tamanio_bitarray);

//	if(archivo_bitarray) {
//		// si ya existe se escribe en el creado lo que hay en el archivo
//		// SETEO EL PUNTERO AL PRINCIPIO
//		fseek(archivo_bitarray, 0, SEEK_SET);
//		// PASO LA INFO DEL ARCHIVO AL BITARRAY
//		fread(bitarray_mem, tamanio_bitarray, 1, archivo_bitarray);
//		msync(bitarray_mem, tamanio_bitarray, MS_SYNC);
//	}



	fclose(archivo_bloques);

}


void iniciar_bitmap(int tamanio_bitarray){

	void* puntero_bitmap = malloc(tamanio_bitarray);
	//char buffer[tamanio_bitarray];
	memset(puntero_bitmap, 0, tamanio_bitarray);
	bitarray_mem = bitarray_create_with_mode(puntero_bitmap, tamanio_bitarray, LSB_FIRST);

	FILE* archivo_bitarray = fopen(path_bitarray, "wb+");
	fflush(archivo_bitarray);
	ftruncate(fileno(archivo_bitarray), tamanio_bitarray);

	bitarray_mem = mmap(NULL, tamanio_bitarray, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(archivo_bitarray), 0);
	// aca mapeo lo que hay en el arhcivo al bitarray_mem, ya esta la info ahi. el bitmap creado estaba en cero, ahora si el archivo tenia cosas, al hacer el mmap entiendo que agarra la info de ahi

	msync(bitarray_mem, tamanio_bitarray, MS_SYNC);

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
    			bloque_final = bloque_inicial + (tamanio/app_config->block_size);
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
void create(char* nombre, char* path){

	int primer_bloque = buscar_primer_bloque_bitmap_libre();
	ocupar_bloques_bitmap(1, primer_bloque);

	crear_metadata(path, primer_bloque);

	agregar_a_lista_metadata(nombre, primer_bloque, primer_bloque, 0);
}

void delete(char* nombre, char* path){
	t_config* metadata = leer_metadata(path);
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
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre);

	int bloque_inicial = metadata->bloque_inicial;
	int bloque_final = metadata->bloque_final;
	int cantidad_bloques_nueva = nuevo_tamanio/app_config->block_size;
	int nuevo_bloque_final = bloque_inicial + cantidad_bloques_nueva - 1;

	//int cantidad_bloques_necesitada =  cantidad_bloques_nueva -


	if(nuevo_tamanio > metadata->tamanio){
		extender_tamanio_archivo(metadata, bloque_final + 1, &nuevo_bloque_final, nuevo_tamanio);
	} else {
		desocupar_bloques_bitmap(nuevo_bloque_final + 1, bloque_final);
	}
	// actualizar metadata lista, actualizar .config
	actualizar_metadata(metadata, metadata->bloque_inicial, nuevo_bloque_final, nuevo_tamanio);


}

void extender_tamanio_archivo(t_metadata* metadata, int bloque_final, int* nuevo_bloque_final, int nuevo_tamanio){
	int nuevo_bloque_final_int = *nuevo_bloque_final;
	if(hay_bloques_contiguos_disponibles(nuevo_bloque_final_int - bloque_final) < 0){
		compactacion(metadata);
		nuevo_bloque_final_int = metadata->bloque_inicial + nuevo_tamanio/app_config->block_size - 1;
	}

	ocupar_bloques_bitmap(bloque_final, nuevo_bloque_final_int);
}

int buscar_primer_bloque_bitmap_libre(){
	int bloque = 0;
	while(bitarray_test_bit(bitarray, bloque)){
		bloque++;
	}

	return bloque;
}

void ocupar_bloques_bitmap(int bloque_inicial, int bloque_final){
	//FILE* archivo_bitarray = fopen("bitmap.dat","w+");
	for(int i=bloque_inicial; i <=bloque_final; i++){
		bitarray_set_bit(bitarray_mem, i);
		//fseek(archivo_bitarray, i, SEEK_SET);
		// setea un solo bit en 1, no se si esta bien esto
		//fwrite(1,1,1,archivo_bitarray);
		msync(bitarray_mem,tamanio_bitarray, MS_SYNC);

		//fclose(archivo_bitarray);
	}
}

void desocupar_bloques_bitmap(int bloque_inicial, int bloque_final){
	//FILE* archivo_bitarray = fopen("bitmap.dat", "w+");
	for(int i=bloque_inicial; i <= bloque_final; i++){
		//fseek(archivo_bitarray, i, SEEK_SET);
		bitarray_clean_bit(bitarray_mem, i);
		//fwrite(0,1,1, archivo_bitarray);
		msync(bitarray_mem, tamanio_bitarray, MS_SYNC);
		//fclose(archivo_bitarray);
	}
}

void crear_metadata(char* path, int primer_bloque){
	FILE* archivo_metadata = fopen(path, "w");
	fclose(archivo_metadata);

	t_config *metadata = config_create(path);

	char primer_bloque_str[12];  // Suficientemente grande para contener el valor máximo de un uint32_t
	sprintf(primer_bloque_str, "%u", primer_bloque);
	config_set_value(metadata, "BLOQUE_INICIAL", primer_bloque_str);
	config_set_value(metadata, "TAMANIO_ARCHIVO", "0");

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
	list_remove_and_destroy_by_condition(lista_metadata,encontrar_por_nombre,eliminar_metadata);
}

void actualizar_metadata(t_metadata* metadata, int nuevo_bloque_inicial, int nuevo_bloque_final, int nuevo_tamanio){
	metadata->bloque_inicial = nuevo_bloque_inicial;
	metadata->bloque_final = nuevo_bloque_final;
	metadata->tamanio = nuevo_tamanio;

	t_config *metadata_config = config_create(metadata->nombre);


	// hay que pasar el int a string
	char primer_bloque_str[12];  // Suficientemente grande para contener el valor máximo de un uint32_t
	sprintf(primer_bloque_str, "%u", nuevo_bloque_inicial);
	char nuevo_tamanio_str[12];
	sprintf(nuevo_tamanio_str, "%u", nuevo_tamanio);
	config_set_value(metadata_config, "TAMANIO", nuevo_tamanio_str);
	config_set_value(metadata_config, "PRIMER_BLOQUE", primer_bloque_str);

	config_save(metadata_config);
	config_destroy(metadata_config);
}


int buscar_primer_archivo_desde(int bloque){
	size_t tamanio_bitmap = bitarray_get_max_bit(bitarray);
	while((bloque < tamanio_bitmap) && !bitarray_test_bit(bitarray, bloque)){
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
	//retornar bloques
	//ver con Bruno la lectura y escritura de bloques
	void* buffer_leido = leer_bloques(metadata->tamanio, metadata->bloque_inicial);
	//desocupa bitmap
	desocupar_bloques_bitmap(metadata->bloque_inicial,metadata->bloque_final);

	return buffer_leido;
}

int pegar_y_reubicar(t_metadata* metadata, void* info_binario, int primer_bloque_libre){
	// ocupa bitmap
	ocupar_bloques_bitmap(primer_bloque_libre,metadata->bloque_final);
	// pega la info en el binario (ocupa bloques)
	// devuelve el ultimo bloque usado
	// ACA CAMBIE METADATA->BLOQUE_INICIAL POR PRIMER_BLOQUE_LIBRE PORQ NO ESTBAA ACTUALIZADO
	return escribir_bloques(metadata->tamanio, primer_bloque_libre,metadata->bloque_final,info_binario);
}

//todo
int escribir_bloques(int tamanio, int bloque_inicial, int bloque_final, void* info_binario){
	// revisar este calculo
	int ultimo_bloque_usado = (tamanio/app_config->block_size) + bloque_inicial;
	FILE* archivo_bloques = fopen("bloques.dat", "w");
	fseek(archivo_bloques, bloque_inicial * app_config->block_size, SEEK_SET);
	fwrite(info_binario, tamanio, 1, archivo_bloques);
	return ultimo_bloque_usado;
}
//todo
void* leer_bloques(int tamanio, int bloque_inicial){
	void* buffer_leido = NULL;
	FILE* archivo_bloques = fopen("bloques.dat", "r");
	fseek(archivo_bloques, bloque_inicial * app_config->block_size, SEEK_SET);
	fread(buffer_leido, tamanio, 1, archivo_bloques);
	fclose(archivo_bloques);
	return buffer_leido;
}

int hay_bloques_contiguos_disponibles(int cant_bloques){
    int bitarray_length = bitarray_get_max_bit(bitarray);
    int pos = 0;
    int bloque_libre_inicial = 0;
    int bloques_libres = 0;

    while(pos < bitarray_length && bloques_libres < cant_bloques){
    	bloques_libres = 0;
        bloque_libre_inicial = pos;

        while(!bitarray_test_bit(bitarray, pos)){
            bloques_libres ++;
            pos ++;
        }
        pos ++;
    }

    if(bloques_libres >= cant_bloques){
        return bloque_libre_inicial;
    } else {
        return -1;
    }
}


void compactacion(t_metadata* metadata){
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

}

// aca reescribo leer bloques y escribir bloques pero pasandole el puntero por parametro
void write_fs(char* nombre, int tamanio, int puntero, void* info_a_escribir){
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre);
	FILE* archivo_bloques = fopen("bloques.dat", "w");
	// el puntero es relativo del archivo.
	// tendria que calcular ese lugar en el archivo de datos entero.
	// seria algo como sumar eso a todos los bytes previos antes del archivo
	// o parto del byte inicial y hago eso mas el puntero
	fseek(archivo_bloques, (metadata->bloque_inicial * app_config->block_size) + puntero, SEEK_SET);
	fwrite(info_a_escribir, tamanio, 1, archivo_bloques);
	fclose(archivo_bloques);
}

void* read_fs(char* nombre, int tamanio, int puntero){
	void* info_leida = NULL;
	t_metadata* metadata = buscar_metadata_lista_por_nombre(nombre);
	FILE* archivo_bloques = fopen("bloques.dat", "r");
	// el puntero es relativo del archivo.
	// tendria que calcular ese lugar en el archivo de datos entero.
	// seria algo como sumar eso a todos los bytes previos antes del archivo
	// o parto del byte inicial y hago eso mas el puntero
	fseek(archivo_bloques, (metadata->bloque_inicial * app_config->block_size) + puntero, SEEK_SET);
	fread(info_leida, tamanio, 1, archivo_bloques);
	fclose(archivo_bloques);
	return info_leida;
}

