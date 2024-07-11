#include "../Headers/log.h"

void iniciar_log(char* path) {
	app_log = log_create(path, "Kernel", false, LOG_LEVEL_INFO);

	if(app_log == NULL)
		abort();
}

void logear_creacion_proceso(int pid) {
	log_info(app_log, "Se crea el proceso %d en NEW", pid);
}

void logear_fin_proceso(int pid, char* motivo) {
	log_info(app_log, "Finaliza el proceso %d - Motivo: %s", pid, motivo);
}

void logear_cambio_estado(int pid, char* estado_anterior, char* estado_actual) {
	log_info(app_log, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pid, estado_anterior, estado_actual);
}

void logear_motivo_bloqueo(int pid, char* motivo) {
	log_info(app_log, "PID: %d - Bloqueado por: %s", pid, motivo);
}

void logear_fin_quantum(int pid) {
	log_info(app_log, "PID: %d - Desalojado por fin de Quantum", pid);
}

char* listar_procesos_ready() {
	char* procesos = string_new();

	string_append(&procesos, "COLA READY ");
	string_append(&procesos, app_config->algoritmo_planificacion);
	string_append(&procesos, ":[");

	int contador_iteraciones = 0;

	void append_pid(void* elem) {
		contador_iteraciones++;
		t_pcb* proceso = (t_pcb*) elem;
		char* pid = string_itoa(proceso->pid);
		string_append_with_format(&procesos, "%s", pid);
		if(contador_iteraciones < lista_ready->elements_count) {
			string_append_with_format(&procesos, ",");
		}
		free(pid);
	}
	list_iterate(lista_ready, append_pid);
	string_append(&procesos, "]");
	return procesos;
}

char* listar_procesos_v_ready() {
	char* procesos = string_new();

	string_append(&procesos, "COLA PRIORIDAD READY ");
	string_append(&procesos, app_config->algoritmo_planificacion);
	string_append(&procesos, ":[");

	int contador_iteraciones = 0;

	void append_pid(void* elem) {
		contador_iteraciones++;
		t_pcb* proceso = (t_pcb*) elem;
		char* pid = string_itoa(proceso->pid);
		string_append_with_format(&procesos, "%s", pid);
		if(contador_iteraciones < lista_v_ready->elements_count) {
			string_append_with_format(&procesos, ",");
		}
		free(pid);
	}
	list_iterate(lista_v_ready, append_pid);
	string_append(&procesos, "]");
	return procesos;
}
void logear_ingreso_v_ready() {
	char* lista_pids_v = listar_procesos_v_ready();
	log_info(app_log, "%s", lista_pids_v);
	free(lista_pids_v);
}
void logear_ingreso_ready() {
	char* lista_pids = listar_procesos_ready();
	log_info(app_log, "%s", lista_pids);
	free(lista_pids);
}
