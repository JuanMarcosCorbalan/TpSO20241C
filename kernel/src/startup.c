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
}
