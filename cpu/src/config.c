#include "../Headers/config.h"

void cargar_config(char* path) {
	archivo_config = config_create(path);
	app_config = malloc(sizeof(t_cpu_config));

	app_config->algoritmo_tlb = config_get_string_value(archivo_config, "ALGORITMO_TLB");
	app_config->cantidad_entradas_tlb = config_get_int_value(archivo_config, "CANTIDAD_ENTRADAS_TLB");
	app_config->ip_memoria = config_get_string_value(archivo_config, "IP_MEMORIA");
	app_config->puerto_escucha_dispatch = config_get_int_value(archivo_config, "PUERTO_ESCUCHA_DISPATCH");
	app_config->puerto_escucha_interrupt = config_get_int_value(archivo_config, "PUERTO_ESCUCHA_INTERRUPT");
	app_config->puerto_memoria = config_get_int_value(archivo_config, "PUERTO_MEMORIA");
}

void finalizar_config() {
	free(app_config->algoritmo_tlb);
	free(app_config->ip_memoria);
	free(app_config);
}
