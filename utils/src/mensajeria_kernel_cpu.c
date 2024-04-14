#include "../Headers/mensajeria_kernel_cpu.h"

void request_interrumpir_proceso(int socket, uint32_t pid, uint8_t motivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) + sizeof(uint8_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &motivo, sizeof(uint8_t));

	buffer->stream = stream;

	send_paquete(buffer, MSG_INTERRUPT, socket);
}

dt_interrumpir_proceso* deserializar_interrumpir_proceso(t_buffer* buffer) {
	dt_interrumpir_proceso* interrumpir_proceso = malloc(sizeof(dt_interrumpir_proceso));
	void* stream = buffer->stream;

	memcpy(&interrumpir_proceso->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&interrumpir_proceso->motivo, stream, sizeof(uint8_t));

	return interrumpir_proceso;
}

void request_contexto_proceso(int socket, t_pcb* proceso) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 11;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &proceso->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->program_counter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->quantum, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->quantum_ejecutados, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->estado, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->motivo_blocked, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->motivo_exit, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->algoritmo, sizeof(uint8_t));
	offset += sizeof(uint8_t);

	memcpy(stream + offset, &proceso->registros_cpu->AX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->registros_cpu->BX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->registros_cpu->CX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->registros_cpu->DX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &proceso->registros_cpu->DI, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->registros_cpu->EAX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->registros_cpu->EBX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->registros_cpu->ECX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->registros_cpu->EDX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->registros_cpu->PC, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &proceso->registros_cpu->SI, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_CONTEXTO_PROCESO, socket);
}

dt_contexto_proceso* deserializar_contexto_proceso(t_buffer* buffer) {
	dt_contexto_proceso* contexto_proceso = malloc(sizeof(dt_contexto_proceso));
	contexto_proceso->registros_cpu = malloc(sizeof(t_recurso_sistema));
	void* stream = buffer->stream;

	memcpy(&contexto_proceso->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->program_counter, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->quantum, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->quantum_ejecutados, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->estado, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->motivo_blocked, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->motivo_exit, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->algoritmo, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);

	memcpy(&contexto_proceso->registros_cpu->AX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->registros_cpu->BX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->registros_cpu->CX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->registros_cpu->DX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->registros_cpu->DI, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->registros_cpu->EAX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->registros_cpu->EBX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->registros_cpu->ECX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->registros_cpu->EDX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->registros_cpu->PC, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->registros_cpu->SI, stream, sizeof(uint32_t));


	return contexto_proceso;
}

void request_desalojo_proceso(int socket, dt_contexto_proceso* contexto_proceso) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 11;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &contexto_proceso->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->program_counter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->quantum, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->quantum_ejecutados, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->estado, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->motivo_blocked, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->motivo_exit, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->algoritmo, sizeof(uint8_t));
	offset += sizeof(uint8_t);

	memcpy(stream + offset, &contexto_proceso->registros_cpu->AX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->BX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->CX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->DX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->DI, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->EAX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->EBX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->ECX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->EDX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->PC, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->SI, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_DESALOJO, socket);
}

void request_exit_proceso(int socket, dt_contexto_proceso* contexto_proceso) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 11;
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &contexto_proceso->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->program_counter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->quantum, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->quantum_ejecutados, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->estado, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->motivo_blocked, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->motivo_exit, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->algoritmo, sizeof(uint8_t));
	offset += sizeof(uint8_t);

	memcpy(stream + offset, &contexto_proceso->registros_cpu->AX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->BX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->CX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->DX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->DI, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->EAX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->EBX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->ECX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->EDX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->PC, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto_proceso->registros_cpu->SI, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_FINALIZAR_PROCESO, socket);
}

void request_sleep_proceso(int socket, dt_contexto_proceso* contexto, char* nombre_interfaz, uint32_t unidad_trabajo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 13 + (strlen(nombre_interfaz) + 1);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &contexto->pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->program_counter, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->quantum, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->quantum_ejecutados, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->estado, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->motivo_blocked, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->motivo_exit, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->algoritmo, sizeof(uint8_t));
	offset += sizeof(uint8_t);

	memcpy(stream + offset, &contexto->registros_cpu->AX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->registros_cpu->BX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->registros_cpu->CX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->registros_cpu->DX, sizeof(uint8_t));
	offset += sizeof(uint8_t);
	memcpy(stream + offset, &contexto->registros_cpu->DI, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->registros_cpu->EAX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->registros_cpu->EBX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->registros_cpu->ECX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->registros_cpu->EDX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->registros_cpu->PC, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &contexto->registros_cpu->SI, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, &unidad_trabajo, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	uint32_t tamanio_nombre_interfaz = strlen(nombre_interfaz) + 1;
	memcpy(stream + offset, &tamanio_nombre_interfaz, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, nombre_interfaz, tamanio_nombre_interfaz);
	offset += tamanio_nombre_interfaz;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_GEN_SLEEP, socket);
}

dt_sleep_proceso* deserializar_sleep_proceso(t_buffer* buffer) {
	dt_sleep_proceso* contexto_proceso = malloc(sizeof(dt_sleep_proceso));
	contexto_proceso->contexto_proceso = malloc(sizeof(dt_contexto_proceso));
	contexto_proceso->contexto_proceso->registros_cpu = malloc(sizeof(t_recurso_sistema));
	void* stream = buffer->stream;

	memcpy(&contexto_proceso->contexto_proceso->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->program_counter, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->quantum, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->quantum_ejecutados, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->estado, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->motivo_blocked, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->motivo_exit, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->algoritmo, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);

	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->AX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->BX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->CX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->DX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->DI, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->EAX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->EBX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->ECX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->EDX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->PC, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->contexto_proceso->registros_cpu->SI, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&contexto_proceso->unidad_trabajo, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&contexto_proceso->tamanio_nombre_interfaz, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	contexto_proceso->nombre_interfaz = malloc(contexto_proceso->tamanio_nombre_interfaz);
	memcpy(contexto_proceso->nombre_interfaz, stream, contexto_proceso->tamanio_nombre_interfaz);

	return contexto_proceso;
}

