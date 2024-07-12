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

void request_io_gen_sleep(int socket, char* instruccion, uint32_t unidad_trabajo, uint32_t pid) {
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

	send_paquete(buffer, MSG_IO_GEN_SLEEP, socket);
}

dt_io_sleep* deserializar_io_gen_sleep(t_buffer* buffer) {
	dt_io_sleep* instruccion = malloc(sizeof(dt_io_sleep));
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

void request_desbloquear_proceso_io(int socket, uint32_t pid) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_DESBLOQUEAR_IO, socket);
}

uint32_t deserializar_desbloquear_proceso_io(t_buffer* buffer) {
	void* stream = buffer->stream;

	uint32_t pid;
	memcpy(&pid, stream, sizeof(uint32_t));

	return pid;
}

void request_finalizar_proceso_io(int socket, uint32_t pid) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_FINALIZAR_IO, socket);
}

uint32_t deserializar_finalizar_proceso_io(t_buffer* buffer) {
	void* stream = buffer->stream;

	uint32_t pid;
	memcpy(&pid, stream, sizeof(uint32_t));

	return pid;
}

void request_io_stdin_read(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDIN_READ, socket);
}

void request_io_stdout_write(int socket, uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) * 3;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDOUT_WRITE, socket);
}

dt_io_std* deserializar_io_std(t_buffer* buffer) {
	dt_io_std* io_std = malloc(sizeof(dt_io_std));
	void* stream = buffer->stream;

	memcpy(&io_std->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&io_std->direccion_fisica, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&io_std->tamanio, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	return io_std;
}

void request_iniciar_archivo(int socket, uint32_t pid, char* nombre_archivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_nombre_archivo = strlen(nombre_archivo) + 1;
	buffer->size = sizeof(uint32_t) * 2 + tamanio_nombre_archivo;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_nombre_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, nombre_archivo, tamanio_nombre_archivo);
	offset += tamanio_nombre_archivo;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_CREATE, socket);
}

void request_borrar_archivo(int socket, uint32_t pid, char* nombre_archivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_nombre_archivo = strlen(nombre_archivo) + 1;
	buffer->size = sizeof(uint32_t) * 2 + tamanio_nombre_archivo;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_nombre_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, nombre_archivo, tamanio_nombre_archivo);
	offset += tamanio_nombre_archivo;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_DELETE, socket);
}

dt_fs_name* deserializar_fs_name(t_buffer* buffer) {
	dt_fs_name* fs_name = malloc(sizeof(dt_fs_name));
	void* stream = buffer->stream;

	memcpy(&fs_name->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_name->tamanio_nombre_archivo, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	fs_name->nombre_archivo = malloc(fs_name->tamanio_nombre_archivo);
	memcpy(fs_name->nombre_archivo, stream, fs_name->tamanio_nombre_archivo);

	return fs_name;
}

void request_truncate_archivo(int socket, uint32_t pid, char* nombre_archivo, uint32_t tamanio) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_nombre_archivo = strlen(nombre_archivo) + 1;
	buffer->size = sizeof(uint32_t) * 3 + tamanio_nombre_archivo;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_nombre_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, nombre_archivo, tamanio_nombre_archivo);
	offset += tamanio_nombre_archivo;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_TRUNCATE, socket);
}

dt_fs_truncate* deserializar_truncate_archivo(t_buffer* buffer) {
	dt_fs_truncate* fs_truncate = malloc(sizeof(dt_fs_truncate));
	fs_truncate->fs_name = malloc(sizeof(dt_fs_name));
	void* stream = buffer->stream;

	memcpy(&fs_truncate->fs_name->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_truncate->tamanio, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_truncate->fs_name->tamanio_nombre_archivo, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	fs_truncate->fs_name->nombre_archivo = malloc(fs_truncate->fs_name->tamanio_nombre_archivo);
	memcpy(fs_truncate->fs_name->nombre_archivo, stream, fs_truncate->fs_name->tamanio_nombre_archivo);

	return fs_truncate;
}

void request_leer_archivo(int socket, uint32_t pid, char* nombre_archivo, uint32_t registro_direccion, uint32_t registro_tamanio, uint32_t registro_puntero_archivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_nombre_archivo = strlen(nombre_archivo) + 1;
	buffer->size = sizeof(uint32_t) * 5 + tamanio_nombre_archivo;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &registro_direccion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &registro_tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &registro_puntero_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_nombre_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, nombre_archivo, tamanio_nombre_archivo);
	offset += tamanio_nombre_archivo;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_READ, socket);
}

void request_escribir_archivo(int socket, uint32_t pid, char* nombre_archivo, uint32_t registro_direccion, uint32_t registro_tamanio, uint32_t registro_puntero_archivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	uint32_t tamanio_nombre_archivo = strlen(nombre_archivo) + 1;
	buffer->size = sizeof(uint32_t) * 5 + tamanio_nombre_archivo;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &registro_direccion, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &registro_tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &registro_puntero_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio_nombre_archivo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, nombre_archivo, tamanio_nombre_archivo);
	offset += tamanio_nombre_archivo;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_FS_WRITE, socket);
}

dt_fs_rw* deserializar_fs_rw(t_buffer* buffer) {
	dt_fs_rw* fs_rw = malloc(sizeof(dt_fs_rw));
	fs_rw->fs_name = malloc(sizeof(dt_fs_name));
	void* stream = buffer->stream;

	memcpy(&fs_rw->fs_name->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_rw->registro_direccion, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_rw->registro_tamanio, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_rw->registro_puntero_archivo, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&fs_rw->fs_name->tamanio_nombre_archivo, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	fs_rw->fs_name->nombre_archivo = malloc(fs_rw->fs_name->tamanio_nombre_archivo);
	memcpy(fs_rw->fs_name->nombre_archivo, stream, fs_rw->fs_name->tamanio_nombre_archivo);

	return fs_rw;
}

void request_proceso_io_esperando(int socket, uint32_t estado) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &estado, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_ESPERAR_IO, socket);
}

uint32_t deserializar_proceso_io_esperando(t_buffer* buffer) {
	void* stream = buffer->stream;

	uint32_t pid;
	memcpy(&pid, stream, sizeof(uint32_t));

	return pid;
}
