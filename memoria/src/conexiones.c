#include "../Headers/conexiones.h"

void operar(int *socket_cliente) {
	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete = recv_paquete(*socket_cliente);

		uint32_t pid;
		uint32_t estado_escritura;
		uint32_t valor_registro;
		uint32_t marco;
		uint32_t status_resize;
		dt_iniciar_proceso* iniciar_proceso;
		dt_proxima_instruccion* proxima_instruccion;
		dt_resize_proceso* resize_proceso;
		dt_marco_memoria* marco_memoria;
		dt_mov* mov;
		void* stream_rw;
		t_instruccion* instruccion;
		dt_rw_memoria* rw_memoria;
		dt_copy_string* copy_string;
		char* valor_lectura;

		switch(paquete->codigo_operacion) {
			case MSG_SOLICITUD_TAMANIO_PAGINA:
				sleep(app_config->retardo_respuesta);
				request_tamanio_pagina(*socket_cliente, app_config->tam_pagina);
			break;
			case MSG_INICIAR_PROCESO:
				iniciar_proceso = deserializar_iniciar_proceso(paquete->buffer);
				iniciar_espacio_memoria_proceso(iniciar_proceso->pid);
				cargar_instrucciones(iniciar_proceso->pid, iniciar_proceso->path);
				break;
			case MSG_FINALIZAR_PROCESO:
				pid = deserializar_finalizar_proceso(paquete->buffer);
				remover_instrucciones(pid);
				liberar_marcos_proceso(pid);
				break;
			case MSG_PROXIMA_INSTRUCCION:
				proxima_instruccion = deserializar_proxima_instruccion(paquete->buffer);
				instruccion = obtener_instruccion(proxima_instruccion->pid, proxima_instruccion->program_counter);
				sleep(app_config->retardo_respuesta);
				request_instruccion(*socket_cliente, instruccion);
				break;
			case MSG_RESIZE_PROCESO:
				resize_proceso = deserializar_resize_proceso(paquete->buffer);
				status_resize = operar_resize_proceso(resize_proceso->pid, resize_proceso->size);
				sleep(app_config->retardo_respuesta);
				request_status_resize_proceso(*socket_cliente, status_resize);
				break;
			case MSG_MARCO_PAGINA:
				marco_memoria = deserializar_marco_memoria(paquete->buffer);
				marco = buscar_numero_marco_por_pagina(marco_memoria->pid, marco_memoria->numero_pagina);
				sleep(app_config->retardo_respuesta);
				request_numero_marco_memoria(*socket_cliente, marco);
				free(marco_memoria);
				break;
			case MSG_MOV_IN:
				mov = deserializar_mov(paquete->buffer);
				stream_rw = lectura_memoria(mov->pid, mov->direccion_fisica, sizeof(uint32_t));
				memcpy(&valor_registro, stream_rw, sizeof(uint32_t));
				sleep(app_config->retardo_respuesta);
				request_valor_mov_in(*socket_cliente, valor_registro);
				break;
			case MSG_MOV_OUT:
				mov = deserializar_mov(paquete->buffer);
				stream_rw = malloc(sizeof(uint32_t));
				memcpy(stream_rw, &mov->valor_registro, sizeof(uint32_t));
				estado_escritura = escritura_memoria(mov->pid, mov->direccion_fisica, sizeof(uint32_t), stream_rw);
				sleep(app_config->retardo_respuesta);
				request_status_mov_out(*socket_cliente, estado_escritura);
				free(stream_rw);
				break;
			case MSG_IO_STDIN_READ:
				rw_memoria = deserializar_escritura_memoria(paquete->buffer);
				stream_rw = malloc(rw_memoria->tamanio_read_write);
				memcpy(stream_rw, rw_memoria->valor_std, rw_memoria->tamanio_read_write);
				estado_escritura = escritura_memoria(pid, rw_memoria->direccion_fisica, rw_memoria->tamanio_read_write, stream_rw);
				sleep(app_config->retardo_respuesta);
				request_status_escritura_memoria(*socket_cliente, estado_escritura);
				free(stream_rw);
				break;
			case MSG_IO_STDOUT_WRITE:
				rw_memoria = deserializar_lectura_memoria(paquete->buffer);
				stream_rw = lectura_memoria(pid, rw_memoria->direccion_fisica, rw_memoria->tamanio_read_write);
				valor_lectura = malloc(rw_memoria->tamanio_read_write);
				memcpy(valor_lectura, stream_rw, rw_memoria->tamanio_read_write);
				sleep(app_config->retardo_respuesta);
				request_resultado_lectura_memoria(*socket_cliente, valor_lectura);
				free(valor_lectura);
				break;
			case MSG_COPY_STRING:
				copy_string = deserializar_copy_string(paquete->buffer);
				estado_escritura = operar_copy_string(copy_string->pid, copy_string->direccion_fisica_origen, copy_string->direccion_fisica_destino, copy_string->tamanio);
				sleep(app_config->retardo_respuesta);
				request_status_copy_string(*socket_cliente, estado_escritura);
				break;
			default:
				break;
		}

		switch(paquete->codigo_operacion) {
			case MSG_INICIAR_PROCESO:
				free(iniciar_proceso->path);
				free(iniciar_proceso);
				break;
			case MSG_RESIZE_PROCESO:
				free(resize_proceso);
				break;
			case MSG_MOV_IN:
			case MSG_MOV_OUT:
				free(mov);
				break;
			case MSG_IO_STDIN_READ:
			case MSG_IO_STDOUT_WRITE:
				free(rw_memoria->valor_std);
				free(rw_memoria);
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
