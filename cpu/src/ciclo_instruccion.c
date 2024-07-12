#include "../Headers/ciclo_instruccion.h"

uint8_t convertir_tipo_instruccion(char* instruccion) {
	if(strcmp(instruccion, "SET") == 0)
		return SET;
	else if(strcmp(instruccion, "MOV_IN") == 0)
		return MOV_IN;
	else if(strcmp(instruccion, "MOV_OUT") == 0)
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

int existe_entrada_tlb(uint32_t pid, uint32_t numero_pagina) {
	bool existe_entrada(void* elem) {
		t_entrada* aux_entrada = (t_entrada*) elem;
		if(aux_entrada->pid == pid && aux_entrada->pagina == numero_pagina)
			return 1;
		return 0;
	}

	return list_any_satisfy(lista_tlb, existe_entrada);
}

t_entrada* obtener_entrada_tlb(uint32_t pid, uint32_t numero_pagina) {
	bool buscar_entrada(void* elem) {
		t_entrada* aux_entrada = (t_entrada*) elem;
		if(aux_entrada->pid == pid && aux_entrada->pagina == numero_pagina)
			return 1;
		return 0;
	}

	t_entrada* entrada = list_find(lista_tlb, buscar_entrada);

	if(strcmp(app_config->algoritmo_tlb, "LRU")==0) {
		free(entrada->ultimo_uso);
		entrada->ultimo_uso = temporal_get_string_time("%H:%M:%S:%MS");
	}

	return entrada;
}

void actualizar_tlb(t_entrada* nueva_entrada) {
	if(app_config->cantidad_entradas_tlb > lista_tlb->elements_count) {
		list_add(lista_tlb, nueva_entrada);
		return;
	}

	if(strcmp(app_config->algoritmo_tlb, "FIFO") == 0) {
		t_entrada* entrada_fifo = list_remove(lista_tlb, 0);
		list_add(lista_tlb, nueva_entrada);
		free(entrada_fifo);
	}
	else {
	    t_entrada* entrada_lru = list_get(lista_tlb, 0);

	    int index = 0;
	    int index_change = 0;

	    void obtener_lru(void* elem) {
	    	index += 1;
	    	t_entrada* aux_entrada = (t_entrada*) elem;
	    	if(strcmp(aux_entrada->ultimo_uso, entrada_lru->ultimo_uso) < 0) {
	    		entrada_lru = aux_entrada;
	    		index_change = index - 1;
	    	}
	    }

	    list_iterate(lista_tlb, obtener_lru);
	    list_replace(lista_tlb, index_change, nueva_entrada);
	    free(entrada_lru->ultimo_uso);
	    free(entrada_lru);
	}
}

uint32_t obtener_marco(uint32_t pid, uint32_t numero_pagina) {
	t_entrada* entrada;

	if(existe_entrada_tlb(pid, numero_pagina)) {
		logear_tlb_hit(pid, numero_pagina);
		entrada = obtener_entrada_tlb(pid, numero_pagina);
	}
	else {
		logear_tlb_miss(pid, numero_pagina);
		entrada = malloc(sizeof(t_entrada));
		entrada->pid = pid;
		entrada->pagina = numero_pagina;
		request_marco_memoria(socket_memoria, pid, numero_pagina);
		entrada->marco = deserializar_numero_marco_memoria(socket_memoria);

		if(strcmp(app_config->algoritmo_tlb, "LRU") ==0 )
			entrada->ultimo_uso = temporal_get_string_time("%H:%M:%S:%MS");

		actualizar_tlb(entrada);
	}

	logear_obtener_marco(pid, numero_pagina, entrada->marco);
	return entrada->marco;
}

uint32_t obtener_direccion_fisica(uint32_t pid, uint32_t direccion_logica) {
	uint32_t numero_pagina = floor(direccion_logica / tamanio_pagina);
	uint32_t desplazamiento = direccion_logica - numero_pagina * tamanio_pagina;
	uint32_t numero_marco;

	if(app_config->cantidad_entradas_tlb > 0)
		numero_marco = obtener_marco(pid, numero_pagina);

	else {
		request_marco_memoria(socket_memoria, pid, numero_pagina);
		numero_marco = deserializar_numero_marco_memoria(socket_memoria);
		logear_obtener_marco(pid, numero_pagina, numero_marco);
	}

	return tamanio_pagina * numero_marco + desplazamiento;
}

void ejecutar_proceso(dt_contexto_proceso* contexto_proceso, int socket_cliente) {
	int seguir_ejecutando = 1;

	while(seguir_ejecutando) {

		rafaga_quantum = temporal_create();

		// VALIDAR QUE NO TENGA INTERRUPCION
		if(existe_interrupcion) {
			if(motivo_interrupt_bloqueo != 0) {
				contexto_proceso->motivo_blocked = motivo_interrupt_bloqueo;
				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 0;
				request_desalojo_proceso(socket_cliente, contexto_proceso);
			}
			else {
				contexto_proceso->motivo_exit = motivo_interrupt_exit;
				request_exit_proceso(socket_cliente, contexto_proceso);
			}
			existe_interrupcion = 0;
			seguir_ejecutando = 0;

			temporal_destroy(rafaga_quantum);

			continue;
		}

		if((contexto_proceso->algoritmo == RR || contexto_proceso->algoritmo == VRR)) {

			if(contexto_proceso->quantum_ejecutados > contexto_proceso->quantum) {
				contexto_proceso->motivo_blocked = SIN_MOTIVO_BLOCKED;
				contexto_proceso->motivo_exit = SIN_MOTIVO_EXIT;
				contexto_proceso->quantum_ejecutados = 0;

				request_desalojo_proceso(socket_cliente, contexto_proceso);
				seguir_ejecutando = 0;

				temporal_destroy(rafaga_quantum);
				continue;
			}
		}
		
		// FETCH
		logear_fetch_instruccion(contexto_proceso->pid, contexto_proceso->program_counter);
		request_proxima_instruccion(socket_memoria, contexto_proceso->pid, contexto_proceso->program_counter);
		t_paquete* paquete = recv_paquete(socket_memoria);
		t_instruccion* instruccion_completa = deserializar_instruccion(paquete->buffer);
		uint8_t tipo_instruccion = convertir_tipo_instruccion(instruccion_completa->instruccion);
		char* parametros = string_new();

		contexto_proceso->program_counter += 1;

		// EXECUTE
		uint32_t valor_registro_destino;
		uint32_t valor_registro_origen;
		uint32_t valor_registro_dato;
		uint32_t direccion_fisica;
		uint32_t estado_escritura;
		uint32_t dir_fisica_di;
		uint32_t dir_fisica_si;

		switch(tipo_instruccion) {
			case SET:
				setear_registro(contexto_proceso, instruccion_completa->parametro_1, atoi(instruccion_completa->parametro_2));
				break;
			case MOV_IN:
				direccion_fisica = obtener_direccion_fisica(contexto_proceso->pid, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_2));
				request_mov_in(socket_memoria, contexto_proceso->pid ,direccion_fisica);
				valor_registro_dato = deserializar_valor_mov_in(socket_memoria);
				setear_registro(contexto_proceso, instruccion_completa->parametro_1, valor_registro_dato);
				logear_lectura_memoria(contexto_proceso->pid, direccion_fisica, valor_registro_dato);
				break;
			case MOV_OUT:
				direccion_fisica = obtener_direccion_fisica(contexto_proceso->pid, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_1));
				valor_registro_dato = obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_2);
				request_mov_out(socket_memoria, contexto_proceso->pid, valor_registro_dato, direccion_fisica);
				estado_escritura = deserializar_status_mov_out(socket_memoria);
				if(estado_escritura == 0) {
					contexto_proceso->motivo_exit = INVALID_WRITE;
					request_exit_proceso(socket_cliente, contexto_proceso);
					seguir_ejecutando = 0;
				}
				else
					logear_escritura_memoria(contexto_proceso->pid, direccion_fisica, valor_registro_dato);
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
			case COPY_STRING:
					dir_fisica_si = obtener_direccion_fisica(contexto_proceso->pid, contexto_proceso->registros_cpu->SI);
					dir_fisica_di = obtener_direccion_fisica(contexto_proceso->pid, contexto_proceso->registros_cpu->DI);
					request_copy_string(socket_memoria, contexto_proceso->pid, dir_fisica_si, dir_fisica_di, atoi(instruccion_completa->parametro_1));
					estado_escritura = deserializar_status_copy_string(socket_memoria);
					if(estado_escritura == 0) {
						contexto_proceso->motivo_exit = INVALID_WRITE;
						request_exit_proceso(socket_cliente, contexto_proceso);
						seguir_ejecutando = 0;
					}
				break;
			case WAIT:
				request_wait_recurso(socket_cliente, contexto_proceso, instruccion_completa->parametro_1);
				deserializar_desbloquear_cpu(socket_cliente);
				break;
			case SIGNAL:
				request_signal_recurso(socket_cliente, contexto_proceso, instruccion_completa->parametro_1);
				deserializar_desbloquear_cpu(socket_cliente);
				break;
			case IO_GEN_SLEEP:
				contexto_proceso->motivo_blocked = INTERFAZ;

				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 0;
				else
					contexto_proceso->quantum_ejecutados += (int) temporal_gettime(rafaga_quantum);

				request_sleep_proceso(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, atoi(instruccion_completa->parametro_2));
				seguir_ejecutando = 0;
				break;
			case IO_STDIN_READ:
				direccion_fisica = obtener_direccion_fisica(contexto_proceso->pid, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_2));

				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 0;
				else
					contexto_proceso->quantum_ejecutados += (int) temporal_gettime(rafaga_quantum);

				request_stdin_read(socket_cliente, instruccion_completa->parametro_1, contexto_proceso, direccion_fisica, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_3));
				seguir_ejecutando = 0;
				break;
			case IO_STDOUT_WRITE:
				direccion_fisica = obtener_direccion_fisica(contexto_proceso->pid, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_2));

				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 0;
				else
					contexto_proceso->quantum_ejecutados += (int) temporal_gettime(rafaga_quantum);

				contexto_proceso->motivo_blocked = INTERFAZ;
				request_stdout_write(socket_cliente, instruccion_completa->parametro_1, contexto_proceso, direccion_fisica, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_3));
				seguir_ejecutando = 0;
				break;
			case IO_FS_CREATE:
				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 1;
				contexto_proceso->motivo_blocked = INTERFAZ;
				request_io_create(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, instruccion_completa->parametro_2);
				seguir_ejecutando = 0;
				break;
			case IO_FS_DELETE:
				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 1;
				contexto_proceso->motivo_blocked = INTERFAZ;
				request_io_delete(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, instruccion_completa->parametro_2);
				seguir_ejecutando = 0;
				break;
			case IO_FS_TRUNCATE:
				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 1;
				contexto_proceso->motivo_blocked = INTERFAZ;
				request_io_truncate(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, instruccion_completa->parametro_2, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_3));
				seguir_ejecutando = 0;
				break;
			case IO_FS_WRITE:
				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 1;
				contexto_proceso->motivo_blocked = INTERFAZ;
				direccion_fisica = obtener_direccion_fisica(contexto_proceso->pid, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_3));
				request_io_write(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, instruccion_completa->parametro_2, direccion_fisica, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_4), obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_5));
				seguir_ejecutando = 0;
				break;
			case IO_FS_READ:
				if(contexto_proceso->algoritmo == RR)
					contexto_proceso->quantum_ejecutados = 1;
				contexto_proceso->motivo_blocked = INTERFAZ;
				direccion_fisica = obtener_direccion_fisica(contexto_proceso->pid, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_3));
				request_io_read(socket_cliente, contexto_proceso, instruccion_completa->parametro_1, instruccion_completa->parametro_2, direccion_fisica, obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_4),  obtener_valor_registro(contexto_proceso, instruccion_completa->parametro_5));
				seguir_ejecutando = 0;
				break;
			case RESIZE:
				request_resize_proceso(socket_memoria, contexto_proceso->pid, atoi(instruccion_completa->parametro_1));
				if(deserializar_status_resize_proceso(socket_memoria) == 0) {
					contexto_proceso->motivo_exit = OUT_OF_MEMORY;
					request_exit_proceso(socket_cliente, contexto_proceso);
					seguir_ejecutando = 0;
				}
				break;
			case EXIT:
				contexto_proceso->motivo_exit = SUCCESS;
				request_exit_proceso(socket_cliente, contexto_proceso);
				seguir_ejecutando = 0;
				break;
			default:
				break;
		}

		if((contexto_proceso->algoritmo == RR || contexto_proceso->algoritmo == VRR)) {
			contexto_proceso->quantum_ejecutados += (int) temporal_gettime(rafaga_quantum);
		}

		temporal_destroy(rafaga_quantum);

		switch(tipo_instruccion) {
			case EXIT:
				logear_instruccion_ejecutada(contexto_proceso->pid, instruccion_completa->instruccion, "");
			break;
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

void operar_dispatch(int* socket_cliente) {
	int seguir_operando = 1;

	while(seguir_operando) {
		t_paquete* paquete_inicial = recv_paquete(*socket_cliente);
		dt_contexto_proceso* contexto_proceso = deserializar_contexto_proceso(paquete_inicial->buffer);

		ejecutar_proceso(contexto_proceso, *socket_cliente);

		free(contexto_proceso->registros_cpu);
		free(contexto_proceso);
		free(paquete_inicial->buffer->stream);
		free(paquete_inicial->buffer);
		free(paquete_inicial);
	}

	close(*socket_cliente);
	free(socket_cliente);
}
