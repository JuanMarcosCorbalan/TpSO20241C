#include "../Headers/ciclo_instruccion.h"

uint8_t convertir_tipo_instruccion(char* instruccion) {
	if(strcmp(instruccion, "SET") == 0)
		return SET;
	else if(strcmp(instruccion, "MOVE_IN") == 0)
		return MOV_IN;
	else if(strcmp(instruccion, "MOVE_OUT") == 0)
		return MOV_OUT;
	else if(strcmp(instruccion, "SUM") == 0)
		return SUM;
	else if(strcmp(instruccion, "SUB") == 0)
		return SUB;
	else if(strcmp(instruccion, "JNZ") == 0)
		return JNZ;
	else if(strcmp(instruccion, "RESIZE") == 0)
		return RESIZE;
	else if(strcmp(instruccion, "COPY_STRING") == 0)
		return COPY_STRING;
	else if(strcmp(instruccion, "WAIT") == 0)
		return WAIT;
	else if(strcmp(instruccion, "SIGNAL") == 0)
		return SIGNAL;
	else if(strcmp(instruccion, "IO_GEN_SLEEP") == 0)
		return IO_GEN_SLEEP;
	else if(strcmp(instruccion, "IO_STDIN_READ") == 0)
		return IO_STDIN_READ;
	else if(strcmp(instruccion, "IO_STDOUT_WRITE") == 0)
		return IO_STDOUT_WRITE;
	else if(strcmp(instruccion, "IO_FS_CREATE") == 0)
		return IO_FS_CREATE;
	else if(strcmp(instruccion, "IO_FS_DELETE") == 0)
		return IO_FS_DELETE;
	else if(strcmp(instruccion, "IO_FS_TRUNCATE") == 0)
		return IO_FS_TRUNCATE;
	else if(strcmp(instruccion, "IO_FS_READ") == 0)
		return IO_FS_READ;
	else if(strcmp(instruccion, "IO_FS_WRITE") == 0)
		return IO_FS_WRITE;
	else
		return EXIT;
}

void setear_registro(dt_contexto_proceso* contexto_proceso, char* registro, int valor) {
	if(strcmp(registro, "AX") == 0)
		contexto_proceso->registros_cpu->AX = valor;
	else if(strcmp(registro, "BX") == 0)
		contexto_proceso->registros_cpu->BX = valor;
	else if(strcmp(registro, "CX") == 0)
		contexto_proceso->registros_cpu->CX = valor;
	else if(strcmp(registro, "DX") == 0)
		contexto_proceso->registros_cpu->DX = valor;
	else if(strcmp(registro, "EAX") == 0)
		contexto_proceso->registros_cpu->EAX = valor;
	else if(strcmp(registro, "EBX") == 0)
		contexto_proceso->registros_cpu->EBX = valor;
	else if(strcmp(registro, "ECX") == 0)
		contexto_proceso->registros_cpu->ECX = valor;
	else if(strcmp(registro, "EDX") == 0)
		contexto_proceso->registros_cpu->EDX = valor;
	else if(strcmp(registro, "PC") == 0)
		contexto_proceso->registros_cpu->PC = valor;
	else if(strcmp(registro, "SI") == 0)
		contexto_proceso->registros_cpu->SI = valor;
	else if(strcmp(registro, "DI") == 0)
		contexto_proceso->registros_cpu->DI = valor;
}

int obtener_valor_registro(dt_contexto_proceso* contexto_proceso, char* registro) {
	if(strcmp(registro, "AX") == 0)
		return contexto_proceso->registros_cpu->AX;
	else if(strcmp(registro, "BX") == 0)
		return contexto_proceso->registros_cpu->BX;
	else if(strcmp(registro, "CX") == 0)
		return contexto_proceso->registros_cpu->CX;
	else if(strcmp(registro, "DX") == 0)
		return contexto_proceso->registros_cpu->DX;
	else if(strcmp(registro, "EAX") == 0)
		return contexto_proceso->registros_cpu->EAX;
	else if(strcmp(registro, "EBX") == 0)
		return contexto_proceso->registros_cpu->EBX;
	else if(strcmp(registro, "ECX") == 0)
		return contexto_proceso->registros_cpu->ECX;
	else if(strcmp(registro, "EDX") == 0)
		return contexto_proceso->registros_cpu->EDX;
	else if(strcmp(registro, "PC") == 0)
		return contexto_proceso->registros_cpu->PC;
	else if(strcmp(registro, "SI") == 0)
		return contexto_proceso->registros_cpu->SI;
	else if(strcmp(registro, "DI") == 0)
		return contexto_proceso->registros_cpu->DI;
	return 0;
}

void ejecutar_proceso(dt_contexto_proceso* contexto_proceso, int socket_cliente) {
	int seguir_ejecutando = 1;

	while(seguir_ejecutando) {

		// VALIDAR QUE NO TENGA INTERRUPCION
		// VALIDAR QUE NO TENGA INTERRUPCION
		// VALIDAR QUE NO TENGA INTERRUPCION
		// VALIDAR QUE NO TENGA INTERRUPCION

		if((contexto_proceso->algoritmo == RR || contexto_proceso->algoritmo == VRR) && contexto_proceso->quantum_ejecutados > contexto_proceso->quantum) {
			contexto_proceso->quantum_ejecutados = 1;
			request_desalojo_proceso(socket_cliente, contexto_proceso);
			seguir_ejecutando = 0;
			continue;
		}

		// FETCH
		logear_fetch_instruccion(contexto_proceso->pid, contexto_proceso->program_counter);
		request_proxima_instruccion(socket_memoria, contexto_proceso->pid, contexto_proceso->program_counter);
		t_paquete* paquete = recv_paquete(socket_memoria);
		t_instruccion* instruccion_completa = deserializar_instruccion(paquete->buffer);
		uint8_t tipo_instruccion = convertir_tipo_instruccion(instruccion_completa->instruccion);
		char* parametros = string_new();

		contexto_proceso->program_counter += 1;

		if((contexto_proceso->algoritmo == RR || contexto_proceso->algoritmo == VRR))
			contexto_proceso->quantum_ejecutados += 1;

		// DECODE
		// DECODE
		// DECODE
		// DECODE

		// EXECUTE
		int valor_registro_destino;
		int valor_registro_origen;
		switch(tipo_instruccion) {
			case SET:
				setear_registro(contexto_proceso, instruccion_completa->parametro_1, atoi(instruccion_completa->parametro_2));
				break;
			case MOV_IN:
				break;
			case MOV_OUT:
				break;
			case SUM:
				valor_registro_destino = obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_1);
				valor_registro_origen = obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_2);
				setear_registro(contexto_proceso, instruccion_completa->parametro_1, valor_registro_destino + valor_registro_origen);
				break;
			case SUB:
				valor_registro_destino = obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_1);
				valor_registro_origen = obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_2);
				setear_registro(contexto_proceso, instruccion_completa->parametro_1, valor_registro_destino - valor_registro_origen);
				break;
			case JNZ:
				if(obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_1) != 0)
					contexto_proceso->program_counter = atoi(instruccion_completa->parametro_2);
				break;
			case RESIZE:
				break;
			case COPY_STRING:
				break;
			case WAIT:
				break;
			case SIGNAL:
				break;
			case IO_GEN_SLEEP:
				request_sleep_proceso(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, atoi(instruccion_completa->parametro_2));
				seguir_ejecutando = 0;
				break;
			case IO_STDIN_READ:
				break;
			case IO_STDOUT_WRITE:
				break;
			case IO_FS_CREATE:
				break;
			case IO_FS_DELETE:
				break;
			case IO_FS_TRUNCATE:
				break;
			case IO_FS_WRITE:
				break;
			case IO_FS_READ:
				break;
			case EXIT:
				contexto_proceso->motivo_exit = SUCCESS;
				logear_instruccion_ejecutada(contexto_proceso->pid, instruccion_completa->instruccion, "");
				request_exit_proceso(socket_cliente, contexto_proceso);
				seguir_ejecutando = 0;
				break;
			default:
				break;
		}

		switch(tipo_instruccion) {
			case RESIZE:
			case COPY_STRING:
			case WAIT:
			case SIGNAL:
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_1);
				logear_instruccion_ejecutada(contexto_proceso->pid, instruccion_completa->instruccion, parametros);
				free(instruccion_completa->parametro_1);
				break;
			case SET:
			case MOV_IN:
			case MOV_OUT:
			case SUM:
			case SUB:
			case JNZ:
			case IO_GEN_SLEEP:
			case IO_FS_CREATE:
			case IO_FS_DELETE:
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_1);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_2);
				logear_instruccion_ejecutada(contexto_proceso->pid, instruccion_completa->instruccion, parametros);
				free(instruccion_completa->parametro_1);
				free(instruccion_completa->parametro_2);
				break;
			case IO_STDIN_READ:
			case IO_STDOUT_WRITE:
			case IO_FS_TRUNCATE:
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_1);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_2);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_3);
				logear_instruccion_ejecutada(contexto_proceso->pid, instruccion_completa->instruccion, parametros);
				free(instruccion_completa->parametro_1);
				free(instruccion_completa->parametro_2);
				free(instruccion_completa->parametro_3);
				break;
			case IO_FS_WRITE:
			case IO_FS_READ:
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_1);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_2);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_3);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_4);
				string_append_with_format(&parametros,"%s ", instruccion_completa->parametro_5);
				logear_instruccion_ejecutada(contexto_proceso->pid, instruccion_completa->instruccion, parametros);
				free(instruccion_completa->parametro_1);
				free(instruccion_completa->parametro_2);
				free(instruccion_completa->parametro_3);
				free(instruccion_completa->parametro_4);
				free(instruccion_completa->parametro_5);
				break;
			default:
				break;
		}

		free(instruccion_completa->instruccion);
		free(instruccion_completa);
		free(parametros);

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}
