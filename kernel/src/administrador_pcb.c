#include "../Headers/administrador_pcb.h"

char* convertir_estado_proceso(uint8_t estado) {
	if(estado == NEW)
		return "NEW";
	else if(estado == READY)
		return "READY";
	else if(estado == EXEC)
		return "EXEC";
	else if(estado == BLOCKED)
		return "BLOCKED";
	else if(estado == V_READY)
		return "READY+";
	else
		return "EXIT";
}

uint8_t obtener_algoritmo_planificacion() {
	if(strcmp(app_config->algoritmo_planificacion, "FIFO") == 0)
		return FIFO;
	else if(strcmp(app_config->algoritmo_planificacion, "RR") == 0)
		return RR;
	else
		return VRR;
}

void finalizar_procesos() {
	void fin(void* elem) {
		t_pcb* pcb = elem;
		free(pcb->path_instrucciones);
		free(pcb->registros_cpu);
		free(pcb);
	}
	list_destroy_and_destroy_elements(lista_global, fin);
}

t_registros_cpu* crear_registros_cpu() {
	t_registros_cpu* nuevo_registros_cpu = malloc(sizeof(t_registros_cpu));

	nuevo_registros_cpu->AX = 0;
	nuevo_registros_cpu->BX = 0;
	nuevo_registros_cpu->CX = 0;
	nuevo_registros_cpu->DX = 0;
	nuevo_registros_cpu->DI = 0;
	nuevo_registros_cpu->EAX = 0;
	nuevo_registros_cpu->EBX = 0;
	nuevo_registros_cpu->ECX = 0;
	nuevo_registros_cpu->EDX = 0;
	nuevo_registros_cpu->PC = 0;
	nuevo_registros_cpu->SI = 0;

	return nuevo_registros_cpu;
}

t_pcb* crear_pcb(char* path_instrucciones) {
	t_pcb* nuevo_pcb = malloc(sizeof(t_pcb));

	contador_pid += 1;
	nuevo_pcb->pid = contador_pid;
	nuevo_pcb->estado = NEW;
	nuevo_pcb->program_counter = 1;
	nuevo_pcb->quantum = app_config->quantum;
	nuevo_pcb->quantum_ejecutados = 0;
	nuevo_pcb->algoritmo = obtener_algoritmo_planificacion();
	nuevo_pcb->motivo_blocked = SIN_MOTIVO_BLOCKED;
	nuevo_pcb->motivo_exit = SIN_MOTIVO_EXIT;
	nuevo_pcb->registros_cpu = crear_registros_cpu();
	nuevo_pcb->path_instrucciones = malloc(strlen(path_instrucciones) + 1);
	memcpy(nuevo_pcb->path_instrucciones, path_instrucciones, strlen(path_instrucciones) + 1);

	return nuevo_pcb;
}

t_pcb* obtener_pcb_por_id(int pid) {
	bool remover_por_id(void* elem) {
		t_pcb* aux_proceso = (t_pcb*) elem;
		if(aux_proceso->pid == pid)
			return 1;
		return 0;
	}

	pthread_mutex_lock(&mutex_lista_global);
	t_pcb* proceso = list_find(lista_global, remover_por_id);
	pthread_mutex_unlock(&mutex_lista_global);

	return proceso;
}

void remover_pcb(t_pcb* proceso, uint8_t estado) {
	if(estado == NEW) {
		pthread_mutex_lock(&mutex_lista_new);
		list_remove_element(lista_new, proceso);
		pthread_mutex_unlock(&mutex_lista_new);
	}
	else if(estado == READY) {
		pthread_mutex_lock(&mutex_lista_ready);
		list_remove_element(lista_ready, proceso);
		pthread_mutex_unlock(&mutex_lista_ready);
	}
	else if(estado == V_READY) {
		pthread_mutex_lock(&mutex_lista_v_ready);
		list_remove_element(lista_v_ready, proceso);
		pthread_mutex_unlock(&mutex_lista_v_ready);
	}
	else if(estado == EXEC) {
		pthread_mutex_lock(&mutex_lista_exec);
		list_remove_element(lista_exec, proceso);
		pthread_mutex_unlock(&mutex_lista_exec);
	}
	else if(estado == BLOCKED) {
		pthread_mutex_lock(&mutex_lista_blocked);
		list_remove_element(lista_blocked, proceso);
		proceso->motivo_blocked = SIN_MOTIVO_BLOCKED;
		pthread_mutex_unlock(&mutex_lista_blocked);
	}
	else {
		pthread_mutex_lock(&mutex_lista_exit);
		list_remove_element(lista_exit, proceso);
		pthread_mutex_unlock(&mutex_lista_exit);
	}
}

void agregar_pcb(t_pcb* proceso, uint8_t estado) {
	if(estado == NEW) {
		logear_cambio_estado(proceso->pid, convertir_estado_proceso(proceso->estado), "NEW");
		proceso->estado = NEW;
		pthread_mutex_lock(&mutex_lista_new);
		list_add(lista_new, (void*) proceso);
		pthread_mutex_unlock(&mutex_lista_new);
		logear_ingreso_v_ready();
	}
	else if(estado == READY) {
		logear_cambio_estado(proceso->pid, convertir_estado_proceso(proceso->estado), "READY");
		proceso->estado = READY;
		pthread_mutex_lock(&mutex_lista_ready);
		list_add(lista_ready, (void*) proceso);
		pthread_mutex_unlock(&mutex_lista_ready);
		logear_ingreso_ready();
	}
	else if(estado == V_READY) {
		logear_cambio_estado(proceso->pid, convertir_estado_proceso(proceso->estado), "READY+");
		proceso->estado = V_READY;
		pthread_mutex_lock(&mutex_lista_v_ready);
		list_add(lista_v_ready, (void*) proceso);
		pthread_mutex_unlock(&mutex_lista_v_ready);
		logear_ingreso_v_ready();
	}
	else if(estado == EXEC) {
		logear_cambio_estado(proceso->pid, convertir_estado_proceso(proceso->estado), "EXEC");
		proceso->estado = EXEC;
		pthread_mutex_lock(&mutex_lista_exec);
		list_add(lista_exec, (void*) proceso);
		pthread_mutex_unlock(&mutex_lista_exec);
	}
	else if(estado == BLOCKED) {
		logear_cambio_estado(proceso->pid, convertir_estado_proceso(proceso->estado), "BLOCKED");
		proceso->estado = BLOCKED;
		pthread_mutex_lock(&mutex_lista_blocked);
		list_add(lista_blocked, (void*) proceso);
		pthread_mutex_unlock(&mutex_lista_blocked);
	}
	else {
		logear_cambio_estado(proceso->pid, convertir_estado_proceso(proceso->estado), "EXIT");
		proceso->estado = _EXIT;
		pthread_mutex_lock(&mutex_lista_exit);
		list_add(lista_exit, (void*) proceso);
		pthread_mutex_unlock(&mutex_lista_exit);
	}
}
