#include "../Headers/conexiones.h"

void operar_dispatch(int* socket_cliente) {
	close(*socket_cliente);
	free(socket_cliente);
}

void operar_interrupt(int* socket_cliente) {
	close(*socket_cliente);
	free(socket_cliente);
}

void iniciar_conexion_memoria() {
	socket_memoria = establecer_conexion_cliente(app_config->ip_memoria, app_config->puerto_memoria);
	log_info(app_log, "Se establece conexión con Memoria con el socket id %d", socket_memoria);
	sem_post(&sem_conexiones);
}

void iniciar_servidor_dispatch() {
	socket_escucha_dispatch = crear_socket_escucha(app_config->puerto_escucha_dispatch, 1);

	log_info(app_log, "Se crea conexión para la escucha de Dispatch con el socket id %d", socket_escucha_dispatch);

	int *socket_cliente = malloc(sizeof(int));
	*socket_cliente = accept(socket_escucha_dispatch, NULL, NULL);

	if(*socket_cliente < 0)
		abort();

	log_info(app_log, "Se establece conexión con el cliente a dispatch con el socket id %d", *socket_cliente);

	int estado_comunicacion = recv_handshake(*socket_cliente);

	if(estado_comunicacion == 0)
		abort();

	operar_dispatch(socket_cliente);
}

void iniciar_servidor_interrupt() {
	socket_escucha_interrupt = crear_socket_escucha(app_config->puerto_escucha_interrupt, 1);

	log_info(app_log, "Se crea conexión para la escucha de Interrupt con el socket id %d", socket_escucha_interrupt);

	int *socket_cliente = malloc(sizeof(int));
	*socket_cliente = accept(socket_escucha_interrupt, NULL, NULL);

	if(*socket_cliente < 0)
		abort();

	log_info(app_log, "Se establece conexión con el cliente a interrupt con el socket id %d", *socket_cliente);

	int estado_comunicacion = recv_handshake(*socket_cliente);

	if(estado_comunicacion == 0)
		abort();

	operar_interrupt(socket_cliente);
}

void iniciar_conexiones() {
	sem_init(&sem_conexiones, 0, 0);

	pthread_t thread_memoria;
	pthread_create(&thread_memoria, NULL, (void*) iniciar_conexion_memoria, NULL);
	pthread_detach(thread_memoria);

	sem_wait(&sem_conexiones);

	pthread_t thread_dispatch;
	pthread_create(&thread_dispatch, NULL, (void*) iniciar_servidor_dispatch, NULL);
	pthread_detach(thread_dispatch);

	pthread_t thread_interrupt;
	pthread_create(&thread_interrupt, NULL, (void*) iniciar_servidor_interrupt, NULL);
	pthread_detach(thread_interrupt);
}
