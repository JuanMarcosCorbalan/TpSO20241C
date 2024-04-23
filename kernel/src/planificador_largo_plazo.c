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

		agregar_pcb(proceso, READY);

		sem_post(&sem_lista_ready);

		pthread_mutex_unlock(&mutex_planificador_largo_plazo);
	}
}

void finalizar_por_consola(uint32_t pid) {
	t_pcb* proceso = obtener_pcb_por_id(pid);

	if(proceso->estado == EXEC) {
		request_interrumpir_proceso_exit(socket_cpu_interrupt, pid, SUCCESS);
	}
	else {
		remover_pcb(proceso, proceso->estado);

		if(proceso->estado != NEW)
			sem_post(&sem_grado_multiprogramacion);

		// ME FALTA LIBERAR RECURSOS
		request_finalizar_proceso(socket_memoria, pid);
		agregar_pcb(proceso, _EXIT);

	}
}

void iniciar_planificador_largo_plazo() {
	pthread_t thread_planificador;
	pthread_create(&thread_planificador, NULL, (void*) planificador_largo_plazo, NULL);
	pthread_detach(thread_planificador);
}
