#include "../Headers/conexiones.h"

void operar(int *socket_cliente) {
	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete = recv_paquete(*socket_cliente);

		uint32_t pid;
		dt_iniciar_proceso* iniciar_proceso;
		dt_proxima_instruccion* proxima_instruccion;
		t_instruccion* instruccion;
		dt_resize_proceso* resize_proceso;
		uint32_t status_resize;
		dt_marco_memoria* marco_memoria;
		uint32_t marco;
		switch(paquete->codigo_operacion) {
			case MSG_SOLICITUD_TAMANIO_PAGINA:
				sleep(app_config->retardo_respuesta);
				request_tamanio_pagina(*socket_cliente, app_config->tam_pagina);
			break;
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
			case MSG_RESIZE_PROCESO:
				resize_proceso = deserializar_resize_proceso(paquete->buffer);
				// LE HAGO RESIZE AL PROCESO
				status_resize = 1; // ESTO LO REEMPLAZO POR EL ESTADO DE RESIZE. 1 PARA OK, 0 PARA ERROR
				sleep(app_config->retardo_respuesta);
				request_status_resize_proceso(*socket_cliente, status_resize);
				break;
			case MSG_MARCO_PAGINA:
				marco_memoria = deserializar_marco_memoria(paquete->buffer);
				marco = 1; // LOGICA PARA OBTENER EL MARCO DE LA PAGINA Y LO IGUALO AL MARCO
				sleep(app_config->retardo_respuesta);
				request_numero_marco_memoria(*socket_cliente, marco);
				free(marco_memoria);
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

	while(1) {
		int *socket_cliente = malloc(sizeof(int));
		*socket_cliente = accept(socket_escucha, NULL, NULL);

		if(*socket_cliente < 0)
			abort();

		int estado_comunicacion = recv_handshake(*socket_cliente);
		if(estado_comunicacion == 0)
			break;

		pthread_t hilo_cliente;
		pthread_create(&hilo_cliente, NULL, (void*) operar, socket_cliente);
		pthread_detach(hilo_cliente);
	}
}
