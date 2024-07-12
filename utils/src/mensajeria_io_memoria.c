#include "../Headers/mensajeria_io_memoria.h"

void request_escritura_memoria(int socket, uint32_t pid, uint32_t direccion_fisica, char* valor_std) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3 + strlen(valor_std) + 1;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	uint32_t tamanio_valor_std = strlen(valor_std) + 1;
	memcpy(stream + offset, &tamanio_valor_std, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, valor_std, tamanio_valor_std);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDIN_READ, socket);
}

void request_lectura_memoria(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio_lectura) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_lectura, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDOUT_WRITE, socket);
}

dt_rw_memoria* deserializar_escritura_memoria(t_buffer* buffer) {
	dt_rw_memoria* rw_memoria = malloc(sizeof(dt_rw_memoria));
	void* stream = buffer->stream;

	memcpy(&rw_memoria->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_memoria->direccion_fisica, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_memoria->tamanio_read_write, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	rw_memoria->valor_std = malloc(rw_memoria->tamanio_read_write);
	memcpy(rw_memoria->valor_std, stream, rw_memoria->tamanio_read_write);

	return rw_memoria;
}

dt_rw_memoria* deserializar_lectura_memoria(t_buffer* buffer) {
	dt_rw_memoria* rw_memoria = malloc(sizeof(dt_rw_memoria));
	void* stream = buffer->stream;

	memcpy(&rw_memoria->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_memoria->direccion_fisica, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_memoria->tamanio_read_write, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return rw_memoria;
}

void request_resultado_lectura_memoria(int socket, char* valor_lectura) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) + strlen(valor_lectura) + 1;
	void* stream = malloc(buffer->size);
	int offset = 0;

	uint32_t tamanio_valor_std = strlen(valor_lectura) + 1;
	memcpy(stream + offset, &tamanio_valor_std, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, valor_lectura, tamanio_valor_std);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDOUT_WRITE, socket);
}

void request_status_escritura_memoria(int socket, uint32_t estado) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDIN_READ, socket);
}

uint32_t deserializar_status_escritura_memoria(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;
	uint32_t status_escritura;

	memcpy(&status_escritura, stream, sizeof(uint32_t));

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return status_escritura;
}

char* deserializar_resultado_lectura_memoria(int socket) {
	t_paquete* paquete = recv_paquete(socket);
	void* stream = paquete->buffer->stream;

	uint32_t tamanio_interfaz;
	memcpy(&tamanio_interfaz, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	char* interfaz = malloc(tamanio_interfaz);
	memcpy(interfaz, stream, tamanio_interfaz);

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return interfaz;
}

void request_fs_lectura(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio_lectura) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 4; 
	void* stream = malloc(buffer->size);

	uint32_t tamanio_valor = 0;
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_lectura, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_valor, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_WRITE, socket);
}

void request_fs_escritura(int socket, uint32_t pid, uint32_t direccion_fisica, char* valor){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_valor = strlen(valor);
	uint32_t tamanio_lectura = 0;
	buffer->size = sizeof(uint32_t) * 4 + tamanio_valor;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_lectura, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_valor, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, valor, tamanio_valor);
	offset += tamanio_valor;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_READ, socket);
}

dt_rw_fs* deserializar_rw_fs(t_buffer* buffer){
	dt_rw_fs* rw_fs = malloc(sizeof(dt_rw_fs));
	void* stream = buffer->stream;

	memcpy(&rw_fs->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_fs->direccion_fisica, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_fs->tamanio_lectura, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&rw_fs->tamanio_valor, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	if(rw_fs->tamanio_valor > 0){
		rw_fs->valor = malloc(rw_fs->tamanio_valor);
		memcpy(rw_fs->valor, stream , rw_fs->tamanio_valor);
	}

	return rw_fs;
}

void request_valor_fs_lectura(int socket, char* valor_lectura) {
	
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_valor_lectura = strlen(valor_lectura) + 1;
	buffer->size = sizeof(uint32_t) + tamanio_valor_lectura;
	void* stream = malloc(buffer->size);

	memcpy(stream,&tamanio_valor_lectura,sizeof(uint32_t));
	memcpy(stream+sizeof(uint32_t),valor_lectura,tamanio_valor_lectura);

	buffer->stream = stream;

	send_paquete(buffer,MSG_IO_FS_WRITE,socket);

}

char* deserializar_valor_fs_lectura(int socket) {

	t_paquete* paquete = recv_paquete(socket);
	void *stream = paquete->buffer->stream;

	uint32_t tamanio_valor;
	memcpy(&tamanio_valor,stream,sizeof(uint32_t));
	stream += sizeof(uint32_t);

	char * valor = malloc(tamanio_valor);
	memcpy(valor,stream,tamanio_valor);

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return valor;

}
