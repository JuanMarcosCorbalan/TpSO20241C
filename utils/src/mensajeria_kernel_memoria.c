#include "../Headers/mensajeria_kernel_memoria.h"

void request_iniciar_proceso(int socket, t_pcb* proceso) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(dt_iniciar_proceso) + strlen(proceso->path_instrucciones) + 1;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &proceso->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	uint32_t tamanio_path = strlen(proceso->path_instrucciones) + 1;
	memcpy(stream + offset, &tamanio_path, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, proceso->path_instrucciones, tamanio_path);

	buffer->stream = stream;

	send_paquete(buffer, MSG_INICIAR_PROCESO, socket);
}

dt_iniciar_proceso* deserializar_iniciar_proceso(t_buffer* buffer) {
	dt_iniciar_proceso* iniciar_proceso = malloc(sizeof(dt_iniciar_proceso));

	void* stream = buffer->stream;
	memcpy(&iniciar_proceso->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&iniciar_proceso->tamanio_path, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	iniciar_proceso->path = malloc(iniciar_proceso->tamanio_path);
	memcpy(iniciar_proceso->path, stream, iniciar_proceso->tamanio_path);

	return iniciar_proceso;
}

void request_finalizar_proceso(int socket, uint32_t pid) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));

	buffer->stream = stream;

	send_paquete(buffer, MSG_FINALIZAR_PROCESO, socket);
}

uint32_t deserializar_finalizar_proceso(t_buffer* buffer) {
	uint32_t pid;

	void* stream = buffer->stream;
	memcpy(&pid, stream, sizeof(uint32_t));

	return pid;
}

void request_proceso_bloqueado(int socket, uint32_t estado) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_PROCESO_BLOQUEADO_MM, socket);
}

uint32_t deserializar_proceso_bloqueado(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t estado;

	memcpy(&estado, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return estado;
}
