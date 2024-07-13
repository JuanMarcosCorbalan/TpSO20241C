#include "../Headers/planificador_largo_plazo.h"

void planificador_largo_plazo() {
	int continuar_ejecucion = 1;

	while(continuar_ejecucion) {
		if(planificacion_pausada)
			sem_wait(&sem_planificacion_pausada);

		sem_wait(&sem_lista_new);
		sem_wait(&sem_grado_multiprogramacion);

		pthread_mutex_lock(&mutex_planificador_largo_plazo);

		pthread_mutex_lock(&mutex_lista_new);
		t_pcb* proceso = list_remove(lista_new, 0);
		pthread_mutex_unlock(&mutex_lista_new);

		request_iniciar_proceso(socket_memoria, proceso);
		deserializar_proceso_bloqueado(socket_memoria);

		agregar_pcb(proceso, READY);

		sem_post(&sem_lista_ready);

		pthread_mutex_unlock(&mutex_planificador_largo_plazo);
	}
}

void finalizar_por_consola(uint32_t pid) {
	t_pcb* proceso = obtener_pcb_por_id(pid);

	if(proceso->estado == EXEC) {
		request_interrumpir_proceso_exit(socket_cpu_interrupt, pid, SUCCESS_CONSOLA);
	}

	else if(proceso->estado == _EXIT)
		return;

	else {
		remover_pcb(proceso, proceso->estado);

		if(proceso->estado != NEW || proceso->estado != EXEC)
			sem_post(&sem_grado_multiprogramacion);

		if(proceso->estado == READY)
			sem_wait(&sem_lista_ready);

		agregar_pcb(proceso, _EXIT);
		liberar_recursos(proceso);
		request_finalizar_proceso(socket_memoria, pid);
		deserializar_proceso_bloqueado(socket_memoria);
	}
}

void iniciar_planificador_largo_plazo() {
	pthread_t thread_planificador;
	pthread_create(&thread_planificador, NULL, (void*) planificador_largo_plazo, NULL);
	pthread_detach(thread_planificador);
}
