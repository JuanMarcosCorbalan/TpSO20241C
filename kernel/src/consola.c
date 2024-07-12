#include "../Headers/consola.h"

void listar_procesos(t_list* lista, char* estado) {
	char* procesos = string_new();
	string_append(&procesos, estado);
	string_append(&procesos, ":[");
	int contador_iteraciones = 0;

	void append_pid(void* elem) {
		contador_iteraciones++;
		t_pcb* proceso = (t_pcb*) elem;
		char* pid = string_itoa(proceso->pid);
		string_append_with_format(&procesos, "%s", pid);
		if(contador_iteraciones < lista->elements_count) {
			string_append_with_format(&procesos, ",");
		}
		free(pid);
	}
	list_iterate(lista, append_pid);
	string_append(&procesos, "]");
	printf("%s\n", procesos);
	free(procesos);
}

void multiprogramacion(char* grado) {
	int nuevo_grado = atoi(grado);
	int diferencia = 0;

	if(nuevo_grado > app_config->grado_multiprogramacion) {
		diferencia = nuevo_grado - app_config->grado_multiprogramacion;
		for(int i=0; i<diferencia; i++) {
			sem_post(&sem_grado_multiprogramacion);
		}
	}
	else {
		diferencia = app_config->grado_multiprogramacion - nuevo_grado;
		for(int i=0; i<diferencia; i++) {
			sem_wait(&sem_grado_multiprogramacion);
		}
	}

	free(grado);
}

uint8_t obtener_tipo_operachion(char* operacion) {
	if(strcmp(operacion, "EJECUTAR_SCRIPT") == 0)
		return EJECUTAR_SCRIPT;
	else if(strcmp(operacion, "INICIAR_PROCESO") == 0)
		return INICIAR_PROCESO;
	else if(strcmp(operacion, "FINALIZAR_PROCESO") == 0)
		return FINALIZAR_PROCESO;
	else if(strcmp(operacion, "MULTIPROGRAMACION") == 0)
		return MULTIPROGRAMACION;
	else if(strcmp(operacion, "DETENER_PLANIFICACION") == 0)
		return DETENER_PLANIFICACION;
	else if(strcmp(operacion, "INICIAR_PLANIFICACION") == 0)
		return INICIAR_PLANIFICACION;
	else if(strcmp(operacion, "PROCESO_ESTADO") == 0)
		return PROCESO_ESTADO;
	else if(strcmp(operacion, "FINALIZAR") == 0)
		return FINALIZAR;
	else
		return OPERACION_INCORRECTA;
}

void iniciar_proceso(char* path) {
	pthread_mutex_lock(&mutex_contador_pid);
	t_pcb* nuevo_pcb = crear_pcb(path);
	pthread_mutex_unlock(&mutex_contador_pid);

	pthread_mutex_lock(&mutex_lista_global);
	list_add(lista_global, (void*) nuevo_pcb);
	pthread_mutex_unlock(&mutex_lista_global);

	logear_creacion_proceso(nuevo_pcb->pid);

	pthread_mutex_lock(&mutex_lista_new);
	list_add(lista_new, (void*) nuevo_pcb);
	pthread_mutex_unlock(&mutex_lista_new);
	sem_post(&sem_lista_new);

	free(path);
}

void finalizar_proceso(char* pid) {
	finalizar_por_consola(atoi(pid));
	free(pid);
}

void iniciar_planificacion() {

	void reanudar_ios(void* elem) {
		t_interfaz_io* aux_interfaz = (void*) elem;
		if(!list_is_empty(aux_interfaz->bloqueados) && aux_interfaz->estado_conexion == 1){
			sem_post(&sem_planificacion_pausada);
		}
	}

	list_iterate(lista_interfaces, reanudar_ios);

	if(planificacion_pausada) {
		sem_post(&sem_planificacion_pausada);
		sem_post(&sem_planificacion_pausada);
		planificacion_pausada = 0;
	}
}

void detener_planificacion() {
	planificacion_pausada = 1;
}

void proceso_estado() {
	listar_procesos(lista_new, "NEW");
	listar_procesos(lista_ready, "READY");
	listar_procesos(lista_exec, "EXEC");
	listar_procesos(lista_blocked, "BLOCKED");
	listar_procesos(lista_exit, "EXIT");
}

void ejecutar_instruccion(uint8_t tipo_operacion, char* parametro) {
	switch(tipo_operacion) {
		case INICIAR_PROCESO:
			iniciar_proceso(parametro);
			break;
		case MULTIPROGRAMACION:
			multiprogramacion(parametro);
			break;
		case FINALIZAR_PROCESO:
			finalizar_proceso(parametro);
			break;
		case INICIAR_PLANIFICACION:
			iniciar_planificacion();
			break;
		case DETENER_PLANIFICACION:
			detener_planificacion();
			break;
		case PROCESO_ESTADO:
			proceso_estado();
			break;
		case OPERACION_INCORRECTA:
		default:
			break;
	}
}

void ejecutar_script(char* path) {
	char* cwd = malloc(1024);
    getcwd(cwd, 1024);
    string_append_with_format(&cwd, "%s", path);

	FILE * archivo_instrucciones = fopen(cwd, "rb");

    fseek(archivo_instrucciones, 0, SEEK_END);
    long tamaño = ftell(archivo_instrucciones) + 1;
    rewind(archivo_instrucciones);
	char* buffer_instrucciones = malloc(tamaño);

	while(fgets(buffer_instrucciones, tamaño, archivo_instrucciones)){
		strtok(buffer_instrucciones, "\n");
		char** parametros = string_split(buffer_instrucciones, " ");

		if(strcmp(parametros[0], "\n") == 0) {
			free(parametros);
			continue;
		}

        char* operacion = parametros[0];
		uint8_t tipo_operacion = obtener_tipo_operachion(operacion);

		ejecutar_instruccion(tipo_operacion, parametros[1]);

		free(operacion);
		free(parametros);
	}

	fclose(archivo_instrucciones);
	free(buffer_instrucciones);

	free(cwd);
	free(path);
}

void iniciar_consola() {
	int continuar = 1;
	char* linea;

	while(continuar) {
        linea = readline(">");

        if (!linea)
        	break;

        char** parametros = string_split(linea, " ");
        free(linea);

        char* operacion = parametros[0];
		uint8_t tipo_operacion = obtener_tipo_operachion(operacion);
		free(operacion);

		if(tipo_operacion == EJECUTAR_SCRIPT)
			ejecutar_script(parametros[1]);

		else if(tipo_operacion == FINALIZAR)
			continuar = 0;

		else
			ejecutar_instruccion(tipo_operacion, parametros[1]);

		free(parametros);
	}
}
