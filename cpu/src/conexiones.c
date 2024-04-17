#include "../Headers/conexiones.h"

void operar_dispatch(int* socket_cliente) {
	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete_inicial = recv_paquete(*socket_cliente);
		dt_contexto_proceso* contexto_proceso = deserializar_contexto_proceso(paquete_inicial->buffer);

		ejecutar_proceso(contexto_proceso, *socket_cliente);

		free(contexto_proceso->registros_cpu);
		free(contexto_proceso);
		free(paquete_inicial->buffer->stream);
		free(paquete_inicial->buffer);
		free(paquete_inicial);
	}

	close(*socket_cliente);
	free(socket_cliente);
}

void operar_interrupt(int* socket_cliente) {
	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete = recv_paquete(*socket_cliente);
		dt_interrumpir_proceso* interrumpir_proceso = deserializar_interrumpir_proceso(paquete->buffer);

		existe_interrupcion = 1;

		switch(paquete->codigo_operacion) {
			case MSG_INTERRUPT_BLOQUEAR:
				motivo_interrupt_exit = 0;
				motivo_interrupt_bloqueo = interrumpir_proceso->motivo;
			break;
			case MSG_INTERRUPT_EXIT:
				motivo_interrupt_bloqueo = 0;
				motivo_interrupt_exit = interrumpir_proceso->motivo;
			break;
			default:
				break;
		}

		free(interrumpir_proceso);
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}

	close(*socket_cliente);
	free(socket_cliente);
}

void iniciar_conexion_memoria() {
	socket_memoria = establecer_conexion_cliente(app_config->ip_memoria, app_config->puerto_memoria);

	request_solicitud_tamanio_pagina(socket_memoria);
	tamanio_pagina = deserializar_tamanio_pagina(socket_memoria);
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

	sem_wait(&sem_conexiones);
}
