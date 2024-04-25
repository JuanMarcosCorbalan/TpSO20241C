#include "../Headers/mensajeria_cpu_memoria.h"

void request_proxima_instruccion(int socket, uint32_t pid, uint32_t program_counter) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 2;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &program_counter, sizeof(uint32_t));

	buffer->stream = stream;

	send_paquete(buffer, MSG_PROXIMA_INSTRUCCION, socket);
}

dt_proxima_instruccion* deserializar_proxima_instruccion(t_buffer* buffer) {
	dt_proxima_instruccion* proxima_instruccion = malloc(sizeof(dt_proxima_instruccion));
	void* stream = buffer->stream;

	memcpy(&proxima_instruccion->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&proxima_instruccion->program_counter, stream, sizeof(uint32_t));

	return proxima_instruccion;
}

void request_instruccion(int socket, t_instruccion* instruccion) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 6;
	buffer->size += instruccion->tamanio_instruccion;
	buffer->size += instruccion->tamanio_parametro_1;
	buffer->size += instruccion->tamanio_parametro_2;
	buffer->size += instruccion->tamanio_parametro_3;
	buffer->size += instruccion->tamanio_parametro_4;
	buffer->size += instruccion->tamanio_parametro_5;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &instruccion->tamanio_instruccion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &instruccion->tamanio_parametro_1, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &instruccion->tamanio_parametro_2, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &instruccion->tamanio_parametro_3, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &instruccion->tamanio_parametro_4, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &instruccion->tamanio_parametro_5, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	if(instruccion->tamanio_instruccion > 0) {
		memcpy(stream + offset, instruccion->instruccion, instruccion->tamanio_instruccion);
		offset += instruccion->tamanio_instruccion;
	}

	if(instruccion->tamanio_parametro_1 > 0) {
		memcpy(stream + offset, instruccion->parametro_1, instruccion->tamanio_parametro_1);
		offset += instruccion->tamanio_parametro_1;
	}

	if(instruccion->tamanio_parametro_2 > 0) {
		memcpy(stream + offset, instruccion->parametro_2, instruccion->tamanio_parametro_2);
		offset += instruccion->tamanio_parametro_2;
	}

	if(instruccion->tamanio_parametro_3 > 0) {
		memcpy(stream + offset, instruccion->parametro_3, instruccion->tamanio_parametro_3);
		offset += instruccion->tamanio_parametro_3;
	}

	if(instruccion->tamanio_parametro_4 > 0) {
		memcpy(stream + offset, instruccion->parametro_4, instruccion->tamanio_parametro_4);
		offset += instruccion->tamanio_parametro_4;
	}

	if(instruccion->tamanio_parametro_5 > 0) {
		memcpy(stream + offset, instruccion->parametro_5, instruccion->tamanio_parametro_5);
		offset += instruccion->tamanio_parametro_5;
	}

	buffer->stream = stream;

	send_paquete(buffer, MSG_INSTRUCCION, socket);
}

t_instruccion* deserializar_instruccion(t_buffer* buffer) {
	t_instruccion* instruccion = malloc(sizeof(t_instruccion));
	void* stream = buffer->stream;

	memcpy(&instruccion->tamanio_instruccion, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->tamanio_parametro_1, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->tamanio_parametro_2, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->tamanio_parametro_3, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->tamanio_parametro_4, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->tamanio_parametro_5, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	if(instruccion->tamanio_instruccion > 0) {
		instruccion->instruccion = malloc(instruccion->tamanio_instruccion);
		memcpy(instruccion->instruccion, stream, instruccion->tamanio_instruccion);
		stream += instruccion->tamanio_instruccion;
	}

	if(instruccion->tamanio_parametro_1 > 0) {
		instruccion->parametro_1 = malloc(instruccion->tamanio_parametro_1);
		memcpy(instruccion->parametro_1, stream, instruccion->tamanio_parametro_1);
		stream += instruccion->tamanio_parametro_1;
	}

	if(instruccion->tamanio_parametro_2 > 0) {
		instruccion->parametro_2 = malloc(instruccion->tamanio_parametro_2);
		memcpy(instruccion->parametro_2, stream, instruccion->tamanio_parametro_2);
		stream += instruccion->tamanio_parametro_2;
	}

	if(instruccion->tamanio_parametro_3 > 0) {
		instruccion->parametro_3 = malloc(instruccion->tamanio_parametro_3);
		memcpy(instruccion->parametro_3, stream, instruccion->tamanio_parametro_3);
		stream += instruccion->tamanio_parametro_3;
	}

	if(instruccion->tamanio_parametro_4 > 0) {
		instruccion->parametro_4 = malloc(instruccion->tamanio_parametro_4);
		memcpy(instruccion->parametro_4, stream, instruccion->tamanio_parametro_4);
		stream += instruccion->tamanio_parametro_4;
	}

	if(instruccion->tamanio_parametro_5 > 0) {
		instruccion->parametro_5 = malloc(instruccion->tamanio_parametro_5);
		memcpy(instruccion->parametro_5, stream, instruccion->tamanio_parametro_5);
		stream += instruccion->tamanio_parametro_5;
	}

	return instruccion;
}

void request_tamanio_pagina(int socket, uint32_t tamanio_pagina) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &tamanio_pagina, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_TAMANIO_PAGINA, socket);
}

void request_solicitud_tamanio_pagina(int socket) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	uint32_t tamanio_pagina = 0;
	memcpy(stream + offset, &tamanio_pagina, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_SOLICITUD_TAMANIO_PAGINA, socket);
}

uint32_t deserializar_tamanio_pagina(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t tamanio_pagina;

	memcpy(&tamanio_pagina, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return tamanio_pagina;
}

void request_resize_proceso(int socket, uint32_t pid, uint32_t size) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 2;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &size, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_RESIZE_PROCESO, socket);
}

dt_resize_proceso* deserializar_resize_proceso(t_buffer* buffer) {
	dt_resize_proceso* resize_proceso = malloc(sizeof(dt_resize_proceso));
	void* stream = buffer->stream;

	memcpy(&resize_proceso->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&resize_proceso->size, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return resize_proceso;
}

void request_status_resize_proceso(int socket, uint32_t estado) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_STATUS_RESIZE_PROCESO, socket);
}

uint32_t deserializar_status_resize_proceso(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t status_resize;

	memcpy(&status_resize, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return status_resize;
}

void request_marco_memoria(int socket, uint32_t pid, uint32_t numero_pagina) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 2;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &numero_pagina, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_MARCO_PAGINA, socket);
}

dt_marco_memoria* deserializar_marco_memoria(t_buffer* buffer) {
	dt_marco_memoria* marco_memoria = malloc(sizeof(dt_marco_memoria));
	void* stream = buffer->stream;

	memcpy(&marco_memoria->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&marco_memoria->numero_pagina, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return marco_memoria;
}

void request_numero_marco_memoria(int socket, uint32_t numero_marco) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &numero_marco, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_MARCO_PAGINA, socket);
}

uint32_t deserializar_numero_marco_memoria(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t numero_marco;

	memcpy(&numero_marco, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return numero_marco;
}

void request_mov_in(int socket, uint32_t pid, uint32_t direccion_fisica) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3;
	void* stream = malloc(buffer->size);
	int offset = 0;

	uint32_t valor_registro = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &valor_registro, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	buffer->stream = stream;

	send_paquete(buffer, MSG_MOV_IN, socket);
}

void request_mov_out(int socket, uint32_t pid, uint32_t valor_registro, uint32_t direccion_fisica) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &valor_registro, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	buffer->stream = stream;

	send_paquete(buffer, MSG_MOV_OUT, socket);
}

dt_mov* deserializar_mov(t_buffer* buffer) {
	dt_mov* mov = malloc(sizeof(dt_mov));
	void* stream = buffer->stream;

	memcpy(&mov->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&mov->direccion_fisica, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&mov->valor_registro, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return mov;
}

void request_valor_mov_in(int socket, uint32_t valor) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &valor, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_MOV_IN, socket);
}

uint32_t deserializar_valor_mov_in(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t valor;

	memcpy(&valor, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return valor;
}

void request_status_mov_out(int socket, uint32_t estado) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_MOV_OUT, socket);
}

uint32_t deserializar_status_mov_out(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t status;

	memcpy(&status, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return status;
}

void request_copy_string(int socket, uint32_t pid, uint32_t direccion_fisica_origen, uint32_t direccion_fisica_destino, uint32_t tamanio) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 4;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica_origen, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica_destino, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_COPY_STRING, socket);
}

dt_copy_string* deserializar_copy_string(t_buffer* buffer) {
	dt_copy_string* copy_string = malloc(sizeof(dt_copy_string));
	void* stream = buffer->stream;

	memcpy(&copy_string->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&copy_string->direccion_fisica_origen, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&copy_string->direccion_fisica_destino, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&copy_string->tamanio, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return copy_string;
}

void request_status_copy_string(int socket, uint32_t estado) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_COPY_STRING, socket);
}

uint32_t deserializar_status_copy_string(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t status;

	memcpy(&status, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return status;
}

