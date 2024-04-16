#include "../Headers/consola.h"

void listar_procesos(t_list* lista, char* estado) {
	char* procesos = string_new();
	string_append(&procesos, estado);
	string_append(&procesos, ":[");
	int contador_iteraciones = 0;

	void append_pid(void* elem) {
		contador_iteraciones++;
		t_pcb* proceso = (t_pcb*) elem;
		string_append_with_format(&procesos, "%s",string_itoa(proceso->pid));
		if(contador_iteraciones < lista->elements_count) {
			string_append_with_format(&procesos, ",");
		}
	}
	list_iterate(lista, append_pid);
	string_append(&procesos, "]");
	printf("%s\n", procesos);
	free(procesos);
}

uint8_t obtener_tipo_operachion(char* operacion) {
	if(strcmp(operacion, "EJECUTAR_SCRIPT") == 0)
		return EJECUTAR_SCRIPT;
	else if(strcmp(operacion, "INICIAR_PROCESO") == 0)
		return INICIAR_PROCESO;
	else if(strcmp(operacion, "FINALIZAR_PROCESO") == 0)
		return FINALIZAR_PROCESO;
	else if(strcmp(operacion, "DETENER_PLANIFICACION") == 0)
		return DETENER_PLANIFICACION;
	else if(strcmp(operacion, "INICIAR_PLANIFICACION") == 0)
		return INICIAR_PLANIFICACION;
	else if(strcmp(operacion, "PROCESO_ESTADO") == 0)
		return PROCESO_ESTADO;
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

	pthread_mutex_lock(&mutex_lista_new);
	list_add(lista_new, (void*) nuevo_pcb);
	pthread_mutex_unlock(&mutex_lista_new);
	sem_post(&sem_lista_new);

	logear_creacion_proceso(nuevo_pcb->pid);

	free(path);
}

char* ejecutar_script(char* path) {
	FILE * archivo_instrucciones = fopen(path, "r");
	char* buffer_instrucciones = malloc(100);
	fgets(buffer_instrucciones, 100, archivo_instrucciones);
	fclose(archivo_instrucciones);
	free(path);
	return buffer_instrucciones;
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

		char** parametros_aux;
		if(tipo_operacion == EJECUTAR_SCRIPT) {
			char* linea_aux = ejecutar_script(parametros[1]);
			parametros_aux = string_split(linea_aux, " ");
			free(linea_aux);

			char* operacion_aux = parametros_aux[0];
			tipo_operacion = obtener_tipo_operachion(operacion_aux);
			free(operacion_aux);

			parametros[1] = parametros_aux[1];
			free(parametros_aux);
		}

		switch(tipo_operacion) {
			case INICIAR_PROCESO:
				iniciar_proceso(parametros[1]);
				break;
			case FINALIZAR_PROCESO:
				finalizar_proceso(parametros[1]);
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

		free(parametros);
	}
}
