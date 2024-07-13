#include "../Headers/administrador_io.h"

t_interfaz_io* obtener_interfaz(char* nombre) {
	t_interfaz_io* interfaz = NULL;

	if(list_is_empty(lista_interfaces))
		return NULL;

	bool obtener_por_nombre(void* elem) {
		t_interfaz_io* aux_interfaz = (t_interfaz_io*) elem;
		if(strcmp(aux_interfaz->nombre, nombre) == 0)
			return 1;
		return 0;
	}

	pthread_mutex_lock(&mutex_lista_interfaz);
	interfaz = list_find(lista_interfaces, obtener_por_nombre);
	pthread_mutex_unlock(&mutex_lista_interfaz);

	return interfaz;
}

int validar_disponibilidad_interfaz(char* nombre) {

	t_interfaz_io* interfaz = obtener_interfaz(nombre);

	if(interfaz != NULL && interfaz->estado_conexion == 1)
		return 1;
	return 0;
}

void agregar_interfaz(char* nombre, int* socket) {
	t_interfaz_io* interfaz = obtener_interfaz(nombre);

	if(interfaz != NULL && interfaz->estado_conexion == 0) {
		interfaz->estado_conexion = 1;
		interfaz->socket_io = socket;
		return;
	}

	interfaz = malloc(sizeof(t_interfaz_io));
	interfaz->bloqueados = list_create();
	interfaz->estado_conexion = 1;
	interfaz->nombre = malloc(strlen(nombre) + 1);
	strcpy(interfaz->nombre, nombre);
	interfaz->socket_io = socket;
	interfaz->estado_validacion = 0;
	sem_init(&interfaz->sem_bloqueo_global, 0, 0);
	sem_init(&interfaz->sem_espera_global, 0, 0);
	list_add(lista_interfaces, interfaz);
}

void desbloquear_por_io(t_interfaz_io* interfaz, uint32_t pid) {
	bool remover_por_pid(void* elem) {
		t_pcb* aux_pcb = (t_pcb*) elem;
		if(aux_pcb->pid == pid)
			return 1;
		return 0;
	}

	if(planificacion_pausada)
		sem_wait(&sem_planificacion_pausada);

	t_pcb* proceso_desbloqueado = list_remove_by_condition(interfaz->bloqueados, remover_por_pid);

	if(proceso_desbloqueado == NULL)
		return;

	remover_pcb(proceso_desbloqueado, proceso_desbloqueado->estado);

	if(strcmp(app_config->algoritmo_planificacion, "VRR") == 0 && proceso_desbloqueado->quantum_ejecutados <= app_config->quantum && proceso_desbloqueado->quantum_ejecutados > 0) {
		agregar_pcb(proceso_desbloqueado, V_READY);
	}
	else {
	agregar_pcb(proceso_desbloqueado, READY);
	}

	sem_post(&sem_lista_ready);
}

void finalizar_por_io(t_interfaz_io* interfaz, uint32_t pid) {
	bool remover_por_pid(void* elem) {
		t_pcb* aux_pcb = (t_pcb*) elem;
		if(aux_pcb->pid == pid)
			return 1;
		return 0;
	}

	if(planificacion_pausada)
		sem_wait(&sem_planificacion_pausada);

	t_pcb* proceso_desbloqueado = list_remove_by_condition(interfaz->bloqueados, remover_por_pid);

	if(proceso_desbloqueado == NULL)
		return;

	remover_pcb(proceso_desbloqueado, proceso_desbloqueado->estado);
	logear_fin_proceso(pid, "INVALID_WRITE");
	agregar_pcb(proceso_desbloqueado, EXIT);
	sem_post(&sem_grado_multiprogramacion);
}

void operar_io(int* socket_io) {
	t_paquete* paquete_inicial = recv_paquete(*socket_io);
	dt_iniciar_interfaz* iniciar_interfaz = deserializar_iniciar_interfaz(paquete_inicial->buffer);

	agregar_interfaz(iniciar_interfaz->nombre, socket_io);
	t_interfaz_io* interfaz = obtener_interfaz(iniciar_interfaz->nombre);
	free(iniciar_interfaz->nombre);
	free(iniciar_interfaz);

	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete = recv_paquete(*socket_io);
		uint32_t pid;

		switch(paquete->codigo_operacion) {
			case MSG_FINALIZAR_IO:
				pid = deserializar_finalizar_proceso_io(paquete->buffer);
				finalizar_por_io(interfaz, pid);
			break;
			case MSG_DESBLOQUEAR_IO:
				pid = deserializar_desbloquear_proceso_io(paquete->buffer);
				desbloquear_por_io(interfaz, pid);
				break;
			case MSG_ESTADO_VALIDAR_INTERFAZ:
				interfaz->estado_validacion = deserializar_estado_validacion_instruccion(paquete->buffer);
				sem_post(&interfaz->sem_bloqueo_global);
				break;
			case MSG_ESPERAR_IO:
				pid = deserializar_proceso_io_esperando(paquete->buffer);
				sem_post(&interfaz->sem_espera_global);
				break;
			default:
				interfaz->estado_conexion = 0;
				seguir_operando = 0;
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}

	free(paquete_inicial->buffer->stream);
	free(paquete_inicial->buffer);
	free(paquete_inicial);

	close(*socket_io);
	free(socket_io);
}
