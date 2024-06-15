#include "../Headers/conexiones.h"

void iniciar_conexion_memoria() {
	socket_memoria = establecer_conexion_cliente(app_config->ip_memoria, app_config->puerto_memoria);
	sem_post(&sem_conexiones);
}

void iniciar_conexion_kernel() {
	socket_kernel = establecer_conexion_cliente(app_config->ip_kernel, app_config->puerto_kernel);
	sem_post(&sem_conexiones);
}

void iniciar_conexiones() {

	sem_init(&sem_conexiones, 0, 0);

	pthread_t thread_memoria;
	pthread_create(&thread_memoria, NULL, (void*) iniciar_conexion_memoria, NULL);
	pthread_detach(thread_memoria);

	pthread_t thread_kernel;
	pthread_create(&thread_kernel, NULL, (void*) iniciar_conexion_kernel, NULL);
	pthread_detach(thread_kernel);

	sem_wait(&sem_conexiones);
	sem_wait(&sem_conexiones);
}
