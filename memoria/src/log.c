#include "../Headers/log.h"

void iniciar_log(char* path) {
	app_log = log_create(path, "Memoria", false, LOG_LEVEL_INFO);

	if(app_log == NULL)
		abort();
}

void logear_tabla_paginas(uint32_t pid, uint32_t cantidad) {
	log_info(app_log, "PID: %d - Tamaño: %d", pid, cantidad);
}

void logear_acceso_tabla_paginas(uint32_t pid, uint32_t pagina, uint32_t marco) {
	log_info(app_log, "PID: %d - Pagina: %d - Marco: %d", pid, pagina, marco);
}

void logear_ampliacion_proceso(uint32_t pid, uint32_t tamanio_actual, uint32_t tamanio_nuevo) {
	log_info(app_log, "PID: %d - Tamaño Actual: %d - Tamaño a Ampliar: %d", pid, tamanio_actual, tamanio_nuevo);
}

void logear_reduccion_proceso(uint32_t pid, uint32_t tamanio_actual, uint32_t tamanio_nuevo) {
	log_info(app_log, "PID: %d - Tamaño Actual: %d - Tamaño a Reducir: %d", pid, tamanio_actual, tamanio_nuevo);
}

void logear_lectura_espacio_usuario(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio) {
	log_info(app_log, "PID: %d - Accion: LEER - Direccion fisica: %d - Tamaño %d", pid, direccion_fisica, tamanio);
}

void logear_escritura_espacio_usuario(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio) {
	log_info(app_log, "PID: %d - Accion: ESCRIBIR - Direccion fisica: %d - Tamaño %d", pid, direccion_fisica, tamanio);
}
