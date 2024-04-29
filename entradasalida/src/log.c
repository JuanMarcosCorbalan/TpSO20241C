#include "../Headers/log.h"

void iniciar_log(char* path) {
	app_log = log_create(path, "IO", false, LOG_LEVEL_INFO);

	if(app_log == NULL)
		abort();
}

char* obtener_operacion(uint32_t operacion) {
	if(operacion == MSG_IO_GEN_SLEEP)
		return "MSG_IO_GEN_SLEEP";
	else if(operacion == MSG_IO_GEN_SLEEP)
		return "MSG_IO_GEN_SLEEP";
	else if(operacion == MSG_IO_STDIN_READ)
		return "MSG_IO_STDIN_READ";
	else if(operacion == MSG_IO_STDOUT_WRITE)
		return "MSG_IO_STDOUT_WRITE";
	return "";
}

void logear_operacion(uint32_t pid, uint32_t operacion) {
	log_info(app_log, "PID: %d - Operacion: %s", pid, obtener_operacion(operacion));
}

void logear_creacion_archivo(uint32_t pid, char* nombre) {
	log_info(app_log, "PID: %d - Crear Archivo: %s", pid, nombre);
}

void logear_eliminar_archivo(uint32_t pid, char* nombre) {
	log_info(app_log, "PID: %d - Eliminar Archivo: %s", pid, nombre);
}
