#include "../Headers/planificador_corto_plazo.h"

void finalizar(t_pcb* proceso) {
	remover_pcb(proceso, proceso->estado);
	// ME FALTA LIBERAR RECURSOS
	request_finalizar_proceso(socket_memoria, proceso->pid);
	agregar_pcb(proceso, _EXIT);
	sem_post(&sem_grado_multiprogramacion);
}

void bloquear(t_pcb* proceso) {
	remover_pcb(proceso, proceso->estado);
	agregar_pcb(proceso, BLOCKED);
}

void actualizar_contexto_pcb(dt_contexto_proceso* contexto_nuevo, t_pcb* proceso) {
	proceso->motivo_blocked = contexto_nuevo->motivo_blocked;
	proceso->motivo_exit = contexto_nuevo->motivo_exit;
	free(proceso->registros_cpu);
	proceso->registros_cpu = contexto_nuevo->registros_cpu;
	proceso->program_counter = contexto_nuevo->program_counter;
	proceso->quantum_ejecutados = contexto_nuevo->quantum_ejecutados;
}

t_pcb* obtener_siguiente_proceso() {
	if(strcmp(app_config->algoritmo_planificacion, "VRR") == 0)
		return (t_pcb*) list_remove(lista_ready, 0);
	return (t_pcb*) list_remove(lista_ready, 0);
}

void ejecutar_proceso(t_pcb* proceso) {
	int seguir_operando = 1;

	request_contexto_proceso(socket_cpu_dispatch, proceso);

	while(seguir_operando) {
		t_paquete* paquete = recv_paquete(socket_cpu_dispatch);

		dt_sleep_proceso* sleep_proceso;
		dt_contexto_proceso* contexto_proceso;
		t_interfaz_io* aux_interfaz;

		switch(paquete->codigo_operacion) {
			case MSG_DESALOJO:
				contexto_proceso = deserializar_contexto_proceso(paquete->buffer);
				actualizar_contexto_pcb(contexto_proceso, proceso);
				logear_fin_quantum(proceso->pid);
				remover_pcb(proceso, proceso->estado);
				agregar_pcb(proceso, READY);
				logear_ingreso_ready();
				sem_post(&sem_lista_ready);
				seguir_operando = 0;
				break;
			case MSG_IO_GEN_SLEEP:
				sleep_proceso = deserializar_sleep_proceso(paquete->buffer);
				sleep_proceso->contexto_proceso->quantum_ejecutados = 1;
				actualizar_contexto_pcb(sleep_proceso->contexto_proceso, proceso);

				if(!validar_disponibilidad_interfaz(sleep_proceso->nombre_interfaz)) {
					finalizar(proceso);
					seguir_operando = 0;
					break;
				}

				aux_interfaz = obtener_interfaz(sleep_proceso->nombre_interfaz);
				request_validar_interfaz(*aux_interfaz->socket_io, "IO_GEN_SLEEP");

				sem_wait(&aux_interfaz->sem_bloqueo_global);

				if(aux_interfaz->estado_validacion == 0) {
					finalizar(proceso);
					seguir_operando = 0;
					break;
				}

				request_ejecutar_instruccion(*aux_interfaz->socket_io, "IO_GEN_SLEEP", sleep_proceso->unidad_trabajo, proceso->pid);
				list_add(aux_interfaz->bloqueados, proceso);
				bloquear(proceso);
				seguir_operando = 0;
				break;
			case MSG_FINALIZAR_PROCESO:
				contexto_proceso = deserializar_contexto_proceso(paquete->buffer);
				actualizar_contexto_pcb(contexto_proceso, proceso);
				finalizar(proceso);
				seguir_operando = 0;
				break;
			default:
				break;
		}

		switch(paquete->codigo_operacion) {
		case MSG_DESALOJO:
		case MSG_FINALIZAR_PROCESO:
			free(contexto_proceso);
			break;
		case MSG_IO_GEN_SLEEP:
			free(sleep_proceso->nombre_interfaz);
			free(sleep_proceso);
			break;
		default:
			break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}

void planificador_corto_plazo() {
	int continuar_ejecucion = 1;

	while(continuar_ejecucion) {
		if(planificacion_pausada)
			sem_wait(&sem_planificacion_pausada);

		sem_wait(&sem_lista_ready);
		pthread_mutex_lock(&mutex_ejecucion_proceso);

		t_pcb* proceso = obtener_siguiente_proceso();
		agregar_pcb(proceso, EXEC);
		ejecutar_proceso(proceso);

		pthread_mutex_unlock(&mutex_ejecucion_proceso);
	}
}

void iniciar_planificador_corto_plazo() {
	pthread_t thread_planificador;
	pthread_create(&thread_planificador, NULL, (void*) planificador_corto_plazo, NULL);
	pthread_detach(thread_planificador);
}
