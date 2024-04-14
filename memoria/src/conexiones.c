#include "../Headers/conexiones.h"

void operar(int *socket_cliente) {
	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete = recv_paquete(*socket_cliente);

		uint32_t pid;
		dt_iniciar_proceso* iniciar_proceso;
		dt_proxima_instruccion* proxima_instruccion;
		t_instruccion* instruccion;
		switch(paquete->codigo_operacion) {
			case MSG_INICIAR_PROCESO:
				iniciar_proceso = deserializar_iniciar_proceso(paquete->buffer);
				cargar_instrucciones(iniciar_proceso->pid, iniciar_proceso->path);
				// CREO LAS ESTRUCTURAS CORRESPONDIENTES
				break;
			case MSG_FINALIZAR_PROCESO:
				pid = deserializar_finalizar_proceso(paquete->buffer);
				// FINALIZO LAS INSTRUCCIONES DEL PROCESO
				// FINALIZO LAS ESTRUCTURAS CORRESPONDIENTES
				break;
			case MSG_PROXIMA_INSTRUCCION:
				proxima_instruccion = deserializar_proxima_instruccion(paquete->buffer);
				instruccion = obtener_instruccion(proxima_instruccion->pid, proxima_instruccion->program_counter);
				sleep(app_config->retardo_respuesta);
				request_instruccion(*socket_cliente, instruccion);
				break;
			default:
				break;
		}

		switch(paquete->codigo_operacion) {
			case MSG_INICIAR_PROCESO:
				free(iniciar_proceso->path);
				free(iniciar_proceso);
				break;
			default:
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}

	close(*socket_cliente);
	free(socket_cliente);
}

void iniciar_servidor() {
	int socket_escucha = crear_socket_escucha(app_config->puerto_escucha, 10);

	log_info(app_log, "Se crea conexión para la escucha con el socket id %d", socket_escucha);

	while(1) {
		int *socket_cliente = malloc(sizeof(int));
		*socket_cliente = accept(socket_escucha, NULL, NULL);

		if(*socket_cliente < 0)
			abort();

		log_info(app_log, "El cliente establece conexión con el socket id %d", *socket_cliente);

		int estado_comunicacion = recv_handshake(*socket_cliente);
		if(estado_comunicacion == 0)
			break;

		pthread_t hilo_cliente;
		pthread_create(&hilo_cliente, NULL, (void*) operar, socket_cliente);
		pthread_detach(hilo_cliente);
	}
}
