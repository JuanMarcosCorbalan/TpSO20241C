#include "../Headers/communication.h"

void send_paquete(t_buffer* buffer, cod_op codigo_operacion, int socket) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_operacion;
	paquete->buffer = buffer;
	void* a_enviar = malloc(buffer->size + sizeof(uint8_t) + sizeof(uint32_t));
	int offset = 0;
	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	send(socket, a_enviar, buffer->size + sizeof(uint8_t) + sizeof(uint32_t), 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

t_paquete* recv_paquete(int socket) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	int contrecv = recv(socket, &(paquete->codigo_operacion), sizeof(uint8_t), MSG_WAITALL);

	if(contrecv < 0)
		abort();

	recv(socket, &(paquete->buffer->size), sizeof(uint32_t), 0);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(socket, paquete->buffer->stream, paquete->buffer->size, 0);

	return paquete;
}

int send_handshake(int socket)
{
	uint8_t request_handshake = HANDSHAKE_OK;
	uint8_t response_handshake;
	send(socket, &request_handshake, sizeof(uint8_t), 0);
	recv(socket, &response_handshake, sizeof(uint8_t), MSG_WAITALL);
	if(response_handshake == HANDSHAKE_OK)
		return 1;
	return 0;
}

int recv_handshake(int socket)
{
	uint8_t request_handshake;
	uint8_t response_handshake;
	recv(socket, &request_handshake, sizeof(uint8_t), MSG_WAITALL);
	if(request_handshake == HANDSHAKE_OK) {
		response_handshake = HANDSHAKE_OK;
		send(socket, &response_handshake, sizeof(uint8_t), 0);
		return 1;
	}
	else {
		response_handshake = HANDSHAKE_NO;
		send(socket, &response_handshake, sizeof(uint8_t), 0);
		return 0;
	}
}

