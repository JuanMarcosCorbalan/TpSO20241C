#include "../Headers/conexiones.h"

void iniciar_conexion_memoria() {
	socket_memoria = establecer_conexion_cliente(app_config->ip_memoria, app_config->puerto_memoria);
	sem_post(&sem_conexiones);
}

void iniciar_conexion_cpu_dispatch() {
	socket_cpu_dispatch = establecer_conexion_cliente(app_config->ip_cpu, app_config->puerto_cpu_dispatch);
	sem_post(&sem_conexiones);
}

void iniciar_conexion_cpu_interrupt() {
	socket_cpu_interrupt = establecer_conexion_cliente(app_config->ip_cpu, app_config->puerto_cpu_interrupt);
	sem_post(&sem_conexiones);
}

void iniciar_escucha_io() {
	socket_escucha_io = crear_socket_escucha(app_config->puerto_escucha, 10);

	while(1) {
		int *socket_io = malloc(sizeof(int));
		*socket_io = accept(socket_escucha_io, NULL, NULL);

		if(*socket_io < 0)
			abort();

		int estado_comunicacion = recv_handshake(*socket_io);
		if(estado_comunicacion == 0)
			break;

		pthread_t hilo_cliente;
		pthread_create(&hilo_cliente, NULL, (void*) operar_io, socket_io);
		pthread_detach(hilo_cliente);
	}
}

void iniciar_conexiones() {

	sem_init(&sem_conexiones, 0, 0);

	pthread_t thread_memoria;
	pthread_create(&thread_memoria, NULL, (void*) iniciar_conexion_memoria, NULL);
	pthread_detach(thread_memoria);

	pthread_t thread_cpu_dispatch;
	pthread_create(&thread_cpu_dispatch, NULL, (void*) iniciar_conexion_cpu_dispatch, NULL);
	pthread_detach(thread_cpu_dispatch);

	pthread_t thread_cpu_interrupt;
	pthread_create(&thread_cpu_interrupt, NULL, (void*) iniciar_conexion_cpu_interrupt, NULL);
	pthread_detach(thread_cpu_interrupt);

	sem_wait(&sem_conexiones);
	sem_wait(&sem_conexiones);
	sem_wait(&sem_conexiones);

	pthread_t thread_io;
	pthread_create(&thread_io, NULL, (void*) iniciar_escucha_io, NULL);
	pthread_detach(thread_io);
}
