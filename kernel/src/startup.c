#include "../Headers/startup.h"

void iniciar_estructuras() {
	contador_pid = 0;
	planificacion_pausada = 0;

	lista_new = list_create();
	lista_ready = list_create();
	lista_exec = list_create();
	lista_blocked = list_create();
	lista_exit = list_create();
	lista_global = list_create();
	lista_interfaces = list_create();
	lista_v_ready = list_create();

	sem_init(&sem_lista_new, 0, 0);
	sem_init(&sem_lista_ready, 0, 0);
	sem_init(&sem_lista_exec, 0, 0);
	sem_init(&sem_lista_blocked, 0, 0);
	sem_init(&sem_grado_multiprogramacion, 0, app_config->grado_multiprogramacion);
	sem_init(&sem_planificacion_pausada, 0, 0);

	pthread_mutex_init(&mutex_lista_new, NULL);
	pthread_mutex_init(&mutex_lista_ready, NULL);
	pthread_mutex_init(&mutex_lista_v_ready, NULL);
	pthread_mutex_init(&mutex_lista_exec, NULL);
	pthread_mutex_init(&mutex_lista_blocked, NULL);
	pthread_mutex_init(&mutex_lista_exit, NULL);
	pthread_mutex_init(&mutex_lista_global, NULL);
	pthread_mutex_init(&mutex_lista_interfaz, NULL);
	pthread_mutex_init(&mutex_contador_pid, NULL);
	pthread_mutex_init(&mutex_ejecucion_proceso, NULL);
	pthread_mutex_init(&mutex_planificador_largo_plazo, NULL);
}

void finalizar_estructuras() {
	pthread_mutex_destroy(&mutex_lista_new);
	pthread_mutex_destroy(&mutex_lista_ready);
	pthread_mutex_destroy(&mutex_lista_v_ready);
	pthread_mutex_destroy(&mutex_lista_exec);
	pthread_mutex_destroy(&mutex_lista_blocked);
	pthread_mutex_destroy(&mutex_lista_exit);
	pthread_mutex_destroy(&mutex_lista_global);
	pthread_mutex_destroy(&mutex_lista_interfaz);
	pthread_mutex_destroy(&mutex_contador_pid);
	pthread_mutex_destroy(&mutex_ejecucion_proceso);
	pthread_mutex_destroy(&mutex_planificador_largo_plazo);

	sem_destroy(&sem_lista_new);
	sem_destroy(&sem_lista_ready);
	sem_destroy(&sem_lista_exec);
	sem_destroy(&sem_lista_blocked);
	sem_destroy(&sem_grado_multiprogramacion);
	sem_destroy(&sem_planificacion_pausada);

	list_destroy(lista_new);
	list_destroy(lista_ready);
	list_destroy(lista_exec);
	list_destroy(lista_blocked);
	list_destroy(lista_exit);
	list_destroy(lista_v_ready);
}
