#include "../Headers/mensajeria_kernel_cpu.h"

void request_interrumpir_proceso_bloquear(int socket, uint32_t pid, uint8_t motivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) + sizeof(uint8_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &motivo, sizeof(uint8_t));

	buffer->stream = stream;

	send_paquete(buffer, MSG_INTERRUPT_BLOQUEAR, socket);
}

void request_interrumpir_proceso_exit(int socket, uint32_t pid, uint8_t motivo) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t) + sizeof(uint8_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &motivo, sizeof(uint8_t));

	buffer->stream = stream;

	send_paquete(buffer, MSG_INTERRUPT_EXIT, socket);
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
	contexto_proceso->registros_cpu = malloc(sizeof(t_registros_cpu));
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
	contexto_proceso->contexto_proceso->registros_cpu = malloc(sizeof(t_registros_cpu));
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

void request_wait_recurso(int socket, dt_contexto_proceso* contexto, char* nombre_recurso) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 12 + (strlen(nombre_recurso) + 1);
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

	uint32_t tamanio_nombre_recurso = strlen(nombre_recurso) + 1;
	memcpy(stream + offset, &tamanio_nombre_recurso, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, nombre_recurso, tamanio_nombre_recurso);
	offset += tamanio_nombre_recurso;

	buffer->stream = stream;

	send_paquete(buffer, MSG_WAIT_RECURSO, socket);
}

void request_signal_recurso(int socket, dt_contexto_proceso* contexto, char* nombre_recurso) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 12 + (strlen(nombre_recurso) + 1);
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

	uint32_t tamanio_nombre_recurso = strlen(nombre_recurso) + 1;
	memcpy(stream + offset, &tamanio_nombre_recurso, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, nombre_recurso, tamanio_nombre_recurso);
	offset += tamanio_nombre_recurso;

	buffer->stream = stream;

	send_paquete(buffer, MSG_SIGNAL_RECURSO, socket);
}

dt_recurso_proceso* deserializar_recurso(t_buffer* buffer) {
	dt_recurso_proceso* contexto_proceso = malloc(sizeof(dt_recurso_proceso));
	contexto_proceso->contexto_proceso = malloc(sizeof(dt_contexto_proceso));
	contexto_proceso->contexto_proceso->registros_cpu = malloc(sizeof(t_registros_cpu));
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

	memcpy(&contexto_proceso->tamanio_nombre_recurso, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	contexto_proceso->nombre_recurso = malloc(contexto_proceso->tamanio_nombre_recurso);
	memcpy(contexto_proceso->nombre_recurso, stream, contexto_proceso->tamanio_nombre_recurso);

	return contexto_proceso;
}

void request_desbloquear_cpu(int socket, uint32_t pid) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint32_t);
	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pid, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	send_paquete(buffer, MSG_DESBLOQUEAR_CPU, socket);
}

uint32_t deserializar_desbloquear_cpu(int socket) {
	t_paquete* paquete = recv_paquete(socket);

	uint32_t pid = 0;
	void* stream = paquete->buffer->stream;

	memcpy(&pid, stream, sizeof(uint32_t));

	return pid;
}

void request_stdin_read(int socket, char* nombre_interfaz, dt_contexto_proceso* contexto, uint32_t direccion_fisica, uint32_t tamanio) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 14 + (strlen(nombre_interfaz) + 1);
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

	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	uint32_t tamanio_nombre_interfaz = strlen(nombre_interfaz) + 1;
	memcpy(stream + offset, &tamanio_nombre_interfaz, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, nombre_interfaz, tamanio_nombre_interfaz);
	offset += tamanio_nombre_interfaz;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDIN_READ, socket);
}

void request_stdout_write(int socket, char* nombre_interfaz, dt_contexto_proceso* contexto, uint32_t direccion_fisica, uint32_t tamanio) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = sizeof(uint8_t) * 8 + sizeof(uint32_t) * 14 + (strlen(nombre_interfaz) + 1);
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

	memcpy(stream + offset, &direccion_fisica, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &tamanio, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	uint32_t tamanio_nombre_interfaz = strlen(nombre_interfaz) + 1;
	memcpy(stream + offset, &tamanio_nombre_interfaz, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(stream + offset, nombre_interfaz, tamanio_nombre_interfaz);
	offset += tamanio_nombre_interfaz;

	buffer->stream = stream;

	send_paquete(buffer, MSG_IO_STDOUT_WRITE, socket);
}

dt_std* deserializar_std(t_buffer* buffer) {
	dt_std* std = malloc(sizeof(dt_std));
	std->contexto_proceso = malloc(sizeof(dt_contexto_proceso));
	std->contexto_proceso->registros_cpu = malloc(sizeof(t_registros_cpu));
	void* stream = buffer->stream;

	memcpy(&std->contexto_proceso->pid, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->program_counter, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->quantum, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->quantum_ejecutados, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->estado, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->motivo_blocked, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->motivo_exit, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->algoritmo, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);

	memcpy(&std->contexto_proceso->registros_cpu->AX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->registros_cpu->BX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->registros_cpu->CX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->registros_cpu->DX, stream, sizeof(uint8_t));
	stream += sizeof(uint8_t);
	memcpy(&std->contexto_proceso->registros_cpu->DI, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->registros_cpu->EAX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->registros_cpu->EBX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->registros_cpu->ECX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->registros_cpu->EDX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->registros_cpu->PC, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->contexto_proceso->registros_cpu->SI, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&std->direccion_fisica, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&std->tamanio, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&std->tamanio_nombre_interfaz, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	std->nombre_interfaz = malloc(std->tamanio_nombre_interfaz);
	memcpy(std->nombre_interfaz, stream, std->tamanio_nombre_interfaz);

	return std;
}

