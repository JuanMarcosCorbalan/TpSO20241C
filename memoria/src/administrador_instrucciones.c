#include "../Headers/administrador_instrucciones.h"

uint8_t convertir_tipo_instruccion(char* instruccion) {
	if(strcmp(instruccion, "SET") == 0)
		return SET;
	else if(strcmp(instruccion, "MOV_IN") == 0)
		return MOV_IN;
	else if(strcmp(instruccion, "MOV_OUT") == 0)
		return MOV_OUT;
	else if(strcmp(instruccion, "SUM") == 0)
		return SUM;
	else if(strcmp(instruccion, "SUB") == 0)
		return SUB;
	else if(strcmp(instruccion, "JNZ") == 0)
		return JNZ;
	else if(strcmp(instruccion, "RESIZE") == 0)
		return RESIZE;
	else if(strcmp(instruccion, "COPY_STRING") == 0)
		return COPY_STRING;
	else if(strcmp(instruccion, "WAIT") == 0)
		return WAIT;
	else if(strcmp(instruccion, "SIGNAL") == 0)
		return SIGNAL;
	else if(strcmp(instruccion, "IO_GEN_SLEEP") == 0)
		return IO_GEN_SLEEP;
	else if(strcmp(instruccion, "IO_STDIN_READ") == 0)
		return IO_STDIN_READ;
	else if(strcmp(instruccion, "IO_STDOUT_WRITE") == 0)
		return IO_STDOUT_WRITE;
	else if(strcmp(instruccion, "IO_FS_CREATE") == 0)
		return IO_FS_CREATE;
	else if(strcmp(instruccion, "IO_FS_DELETE") == 0)
		return IO_FS_DELETE;
	else if(strcmp(instruccion, "IO_FS_TRUNCATE") == 0)
		return IO_FS_TRUNCATE;
	else if(strcmp(instruccion, "IO_FS_READ") == 0)
		return IO_FS_READ;
	else if(strcmp(instruccion, "IO_FS_WRITE") == 0)
		return IO_FS_WRITE;
	else
		return EXIT;
}

uint8_t obtener_cantidad_parametros_instruccion(uint8_t tipo_instruccion) {
	switch(tipo_instruccion) {
		case RESIZE:
		case COPY_STRING:
		case WAIT:
		case SIGNAL:
			return 1;
		break;
		case SET:
		case MOV_IN:
		case MOV_OUT:
		case SUM:
		case SUB:
		case JNZ:
		case IO_GEN_SLEEP:
		case IO_FS_CREATE:
		case IO_FS_DELETE:
			return 2;
		break;
		case IO_STDIN_READ:
		case IO_STDOUT_WRITE:
		case IO_FS_TRUNCATE:
			return 3;
		break;
		case IO_FS_WRITE:
		case IO_FS_READ:
			return 5;
		break;
		default:
			return 0;
		break;
	}
}

t_instruccion* crear_instruccion_vacia() {
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	instruccion->tamanio_instruccion = 0;
	instruccion->tamanio_parametro_1 = 0;
	instruccion->tamanio_parametro_2 = 0;
	instruccion->tamanio_parametro_3 = 0;
	instruccion->tamanio_parametro_4 = 0;
	instruccion->tamanio_parametro_5 = 0;
	return instruccion;
}

void cargar_instrucciones(uint32_t pid, char* path) {
	char* cwd = malloc(1024);
    getcwd(cwd, 1024);
    string_append_with_format(&cwd, "%s", path);

	t_list* instrucciones = list_create();
	FILE * archivo_instrucciones = fopen(cwd, "rb");

    fseek(archivo_instrucciones, 0, SEEK_END);
    long tamaño = ftell(archivo_instrucciones) + 1;
    rewind(archivo_instrucciones);
	char* buffer_instrucciones = malloc(tamaño);

	while(fgets(buffer_instrucciones, tamaño, archivo_instrucciones)){
		strtok(buffer_instrucciones, "\n");
		char** lista_buffer_instrucciones = string_split(buffer_instrucciones, " ");

		if(strcmp(lista_buffer_instrucciones[0], "\n") == 0)
			continue;

		t_instruccion* instruccion = crear_instruccion_vacia();

		uint8_t tipo_instruccion = convertir_tipo_instruccion(lista_buffer_instrucciones[0]);
		uint8_t cantidad_parametros = obtener_cantidad_parametros_instruccion(tipo_instruccion);

		instruccion->tamanio_instruccion = strlen(lista_buffer_instrucciones[0]) + 1;
		instruccion->instruccion = malloc(instruccion->tamanio_instruccion);
		memcpy(instruccion->instruccion, lista_buffer_instrucciones[0], strlen(lista_buffer_instrucciones[0]) + 1);
		free(lista_buffer_instrucciones[0]);

		if(cantidad_parametros >= 1) {
			instruccion->tamanio_parametro_1 = strlen(lista_buffer_instrucciones[1]) + 1;
			instruccion->parametro_1 = malloc(instruccion->tamanio_parametro_1);
			memcpy(instruccion->parametro_1, lista_buffer_instrucciones[1], strlen(lista_buffer_instrucciones[1]) + 1);
			free(lista_buffer_instrucciones[1]);
		}

		if(cantidad_parametros >= 2) {
			instruccion->tamanio_parametro_2 = strlen(lista_buffer_instrucciones[2]) + 1;
			instruccion->parametro_2 = malloc(instruccion->tamanio_parametro_2);
			memcpy(instruccion->parametro_2, lista_buffer_instrucciones[2], strlen(lista_buffer_instrucciones[2]) + 1);
			free(lista_buffer_instrucciones[2]);
		}

		if(cantidad_parametros >= 3) {
			instruccion->tamanio_parametro_3 = strlen(lista_buffer_instrucciones[3]) + 1;
			instruccion->parametro_3 = malloc(instruccion->tamanio_parametro_3);
			memcpy(instruccion->parametro_3, lista_buffer_instrucciones[3], strlen(lista_buffer_instrucciones[3]) + 1);
			free(lista_buffer_instrucciones[3]);
		}

		if(cantidad_parametros >= 4) {
			instruccion->tamanio_parametro_4 = strlen(lista_buffer_instrucciones[4]) + 1;
			instruccion->parametro_4 = malloc(instruccion->tamanio_parametro_4);
			memcpy(instruccion->parametro_4, lista_buffer_instrucciones[4], strlen(lista_buffer_instrucciones[4]) + 1);
			free(lista_buffer_instrucciones[4]);
		}

		if(cantidad_parametros >= 5) {
			instruccion->tamanio_parametro_5 = strlen(lista_buffer_instrucciones[5]) + 1;
			instruccion->parametro_5 = malloc(instruccion->tamanio_parametro_5);
			memcpy(instruccion->parametro_5, lista_buffer_instrucciones[5], strlen(lista_buffer_instrucciones[5]) + 1);
			free(lista_buffer_instrucciones[5]);
		}

		free(lista_buffer_instrucciones);
		list_add(instrucciones, (void*) instruccion);
	}

	free(cwd);
	fclose(archivo_instrucciones);
	free(buffer_instrucciones);

	t_instrucciones_proceso* nuevo_proceso = malloc(sizeof(t_instrucciones_proceso));
	nuevo_proceso->pid = pid;
	nuevo_proceso->instrucciones = instrucciones;
	list_add(instrucciones_procesos, nuevo_proceso);
}

t_instruccion* obtener_instruccion(uint32_t pid, uint32_t program_counter) {
	bool buscar_por_pid(void* elem) {
		t_instrucciones_proceso* aux = (t_instrucciones_proceso*) elem;
		if(aux->pid == pid)
			return 1;
		return 0;
	}
	t_instrucciones_proceso* aux_instruccion_proceso = list_find(instrucciones_procesos, buscar_por_pid);

	return list_get(aux_instruccion_proceso->instrucciones, program_counter - 1);
}

void remover_instrucciones(uint32_t pid) {
	bool buscar_por_pid(void* elem) {
		t_instrucciones_proceso* aux = (t_instrucciones_proceso*) elem;
		if(aux->pid == pid)
			return 1;
		return 0;
	}
	t_instrucciones_proceso* aux_instruccion_proceso = list_remove_by_condition(instrucciones_procesos, buscar_por_pid);

	void eliminar_instruccion(void*elem) {
		t_instruccion* aux_instruccion = (t_instruccion*) elem;
		uint8_t cantidad_parametros = obtener_cantidad_parametros_instruccion(convertir_tipo_instruccion(aux_instruccion->instruccion));

		if(cantidad_parametros >= 1)
			free(aux_instruccion->parametro_1);
		if(cantidad_parametros >= 2)
			free(aux_instruccion->parametro_2);
		if(cantidad_parametros >= 3)
			free(aux_instruccion->parametro_3);
		if(cantidad_parametros >= 4)
			free(aux_instruccion->parametro_4);
		if(cantidad_parametros >= 5)
			free(aux_instruccion->parametro_5);

		free(aux_instruccion->instruccion);
		free(aux_instruccion);
	}

	list_destroy_and_destroy_elements(aux_instruccion_proceso->instrucciones, eliminar_instruccion);
	free(aux_instruccion_proceso);
}

void eliminar_instrucciones() {
	void eliminar_instruccion(void*elem) {
		t_instruccion* aux_instruccion = (t_instruccion*) elem;
		uint8_t cantidad_parametros = obtener_cantidad_parametros_instruccion(convertir_tipo_instruccion(aux_instruccion->instruccion));

		if(cantidad_parametros >= 1)
			free(aux_instruccion->parametro_1);
		if(cantidad_parametros >= 2)
			free(aux_instruccion->parametro_2);
		if(cantidad_parametros >= 3)
			free(aux_instruccion->parametro_3);
		if(cantidad_parametros >= 4)
			free(aux_instruccion->parametro_4);
		if(cantidad_parametros >= 5)
			free(aux_instruccion->parametro_5);

		free(aux_instruccion->instruccion);
		free(aux_instruccion);
	}

	void buscar_por_pid(void* elem) {
		t_instrucciones_proceso* aux = (t_instrucciones_proceso*) elem;
		list_destroy_and_destroy_elements(aux->instrucciones, eliminar_instruccion);
		free(aux);
	}

	list_destroy_and_destroy_elements(instrucciones_procesos, buscar_por_pid);
}
