#include "../Headers/administrador_recurso.h"

void liberar_recursos(t_pcb* proceso) {
	void iterate_recursos(void* elem) {
		t_recurso_sistema* recurso = elem;

		bool tiene_instancia(void* elem) {
			t_pcb* aux_proceso = elem;
			return aux_proceso->pid == proceso->pid;
		}

		if(list_any_satisfy(recurso->asignacion, tiene_instancia)) {
			list_remove_element(recurso->asignacion, proceso);
			recurso->cantidad += 1;
			desbloquear_proceso_recurso(recurso->nombre);
		}

		if(list_any_satisfy(recurso->solicitud, tiene_instancia)) {
			list_remove_element(recurso->solicitud, proceso);
			recurso->cantidad += 1;
		}
	}

	list_iterate(app_config->recursos, iterate_recursos);
}

t_recurso_sistema* obtener_recurso(char* nombre) {
	t_list* recursos = app_config->recursos;

	bool obtener_por_nombre(void* elem) {
		t_recurso_sistema* aux_recurso = (t_recurso_sistema*) elem;
		if(strcmp(aux_recurso->nombre, nombre) == 0)
			return 1;
		return 0;
	}

	t_recurso_sistema* recurso = list_find(recursos, obtener_por_nombre);
	return recurso;
}

int existe_recurso(char* nombre) {
	t_list* recursos = app_config->recursos;

	bool existe_nombre(void* elem) {
		t_recurso_sistema* aux_recurso = (t_recurso_sistema*) elem;
		if(strcmp(aux_recurso->nombre, nombre) == 0)
			return 1;
		return 0;
	}

	return list_any_satisfy(recursos, existe_nombre);
}

int operar_wait(t_pcb* proceso, char* nombre) {
	t_recurso_sistema* recurso = obtener_recurso(nombre);

	if(recurso->cantidad > 0) {
		list_add(recurso->asignacion, proceso);
		recurso->cantidad -= 1;
		return 1;
	}

	list_add(recurso->solicitud, proceso);
	recurso->cantidad -= 1;
	return 0;
}

void desbloquear_proceso_recurso(char* nombre) {
	t_recurso_sistema* recurso = obtener_recurso(nombre);

	if(!list_is_empty(recurso->solicitud)) {
		t_pcb* aux_proceso = list_remove(recurso->solicitud, 0);
		remover_pcb(aux_proceso, aux_proceso->estado);

		if(strcmp(app_config->algoritmo_planificacion, "VRR") == 0 && aux_proceso->quantum_ejecutados <= app_config->quantum && aux_proceso->quantum_ejecutados > 0) {
			agregar_pcb(aux_proceso, V_READY);
		}
		else {
		agregar_pcb(aux_proceso, READY);
		}

		sem_post(&sem_lista_ready);
	}
}

void operar_signal(t_pcb* proceso, char* nombre) {
	t_recurso_sistema* recurso = obtener_recurso(nombre);

	bool remover_por_pid(void* elem) {
		t_pcb* aux_pcb = (t_pcb*) elem;
		if(aux_pcb->pid == proceso->pid) {
			return 1;
		}
		return 0;
	}

	list_remove_by_condition(recurso->asignacion, remover_por_pid);

	recurso->cantidad += 1;
}
