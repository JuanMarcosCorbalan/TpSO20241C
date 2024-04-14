#include "../Headers/mensajeria_kernel_io.h"

void request_iniciar_interfaz(int socket, char* nombre) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) + strlen(nombre) + 1;
	void* stream = malloc(buffer->size);
	int offset = 0;

	uint32_t tamanio_nombre = strlen(nombre) + 1;
	memcpy(stream + offset, &tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, nombre, tamanio_nombre);

	buffer->stream = stream;

	send_paquete(buffer, MSG_INICIAR_INTERFAZ, socket);
}

dt_iniciar_interfaz* deserializar_iniciar_interfaz(t_buffer* buffer) {
	dt_iniciar_interfaz* iniciar_interfaz = malloc(sizeof(dt_iniciar_interfaz));
	void* stream = buffer->stream;

	memcpy(&iniciar_interfaz->tamanio_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	iniciar_interfaz->nombre = malloc(iniciar_interfaz->tamanio_nombre);
	memcpy(iniciar_interfaz->nombre, stream, iniciar_interfaz->tamanio_nombre);

	return iniciar_interfaz;
}

void request_validar_interfaz(int socket, char* instruccion) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) + strlen(instruccion) + 1;
	void* stream = malloc(buffer->size);
	int offset = 0;

	uint32_t tamanio_instruccion = strlen(instruccion) + 1;
	memcpy(stream + offset, &tamanio_instruccion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion, tamanio_instruccion);

	buffer->stream = stream;

	send_paquete(buffer, MSG_VALIDAR_INTERFAZ, socket);
}

char* deserializar_validar_interfaz(t_buffer* buffer) {
	void* stream = buffer->stream;

	uint32_t tamanio_interfaz;
	memcpy(&tamanio_interfaz, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	char* interfaz = malloc(tamanio_interfaz);
	memcpy(interfaz, stream, tamanio_interfaz);

	return interfaz;
}

void request_estado_validacion_instruccion(int socket, uint8_t estado_validacion) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado_validacion, sizeof(uint8_t));

	buffer->stream = stream;

	send_paquete(buffer, MSG_ESTADO_VALIDAR_INTERFAZ, socket);
}

uint8_t deserializar_estado_validacion_instruccion(t_buffer* buffer) {
	void* stream = buffer->stream;

	uint8_t estado_validacion;
	memcpy(&estado_validacion, stream, sizeof(uint8_t));

	return estado_validacion;
}

void request_ejecutar_instruccion(int socket, char* instruccion, uint32_t unidad_trabajo, uint32_t pid) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3 + strlen(instruccion) + 1;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &unidad_trabajo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	uint32_t tamanio_instruccion = strlen(instruccion) + 1;
	memcpy(stream + offset, &tamanio_instruccion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, instruccion, tamanio_instruccion);

	buffer->stream = stream;

	send_paquete(buffer, MSG_EJECUTAR_INTERFAZ, socket);
}

dt_instruccion* deserializar_ejecutar_instruccion(t_buffer* buffer) {
	dt_instruccion* instruccion = malloc(sizeof(dt_instruccion));
	void* stream = buffer->stream;

	memcpy(&instruccion->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->unidad_trabajo, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&instruccion->tamanio_instruccion, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	instruccion->instruccion = malloc(instruccion->tamanio_instruccion);
	memcpy(instruccion->instruccion, stream, instruccion->tamanio_instruccion);

	return instruccion;
}

void request_desbloquear_proceso(int socket, uint32_t pid) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_DESBLOQUEAR_IO, socket);
}

uint32_t deserializar_desbloquear_proceso(t_buffer* buffer) {
	void* stream = buffer->stream;

	uint32_t pid;
	memcpy(&pid, stream, sizeof(uint32_t));

	return pid;
}
