#include "../Headers/log.h"

void iniciar_log(char* path) {
	app_log = log_create(path, "CPU", false, LOG_LEVEL_INFO);

	if(app_log == NULL)
		abort();
}

void logear_fetch_instruccion(uint32_t pid, uint32_t program_counter) {
	log_info(app_log, "PID: %d - FETCH - Program Counter: %d", pid, program_counter);
}

void logear_instruccion_ejecutada(uint32_t pid, char* instruccion, char* parametros) {
	log_info(app_log, "PID: %d - Ejecutando: %s - %s", pid, instruccion, parametros);
}

void logear_tlb_hit(uint32_t pid, uint32_t pagina) {
	log_info(app_log, "PID: %d - TLB HIT - Pagina: %d", pid, pagina);
}

void logear_tlb_miss(uint32_t pid, uint32_t pagina) {
	log_info(app_log, "PID: %d - TLB MISS - Pagina: %d", pid, pagina);
}

void logear_obtener_marco(uint32_t pid, uint32_t pagina, uint32_t marco) {
	log_info(app_log, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", pid, pagina, marco);
}

void logear_escritura_memoria(uint32_t pid, uint32_t direccion, uint32_t valor) {
	log_info(app_log, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %d", pid, direccion, valor);
}
void logear_lectura_memoria(uint32_t pid, uint32_t direccion, uint32_t valor) {
	log_info(app_log, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %d", pid, direccion, valor);
}



