#include "../Headers/config.h"

t_cpu_config* cargar_config(char* path) {
	t_config* archivo_config = config_create(path);
	t_cpu_config* config = malloc(sizeof(t_cpu_config));

	config->algoritmo_tlb = config_get_string_value(archivo_config, "ALGORITMO_TLB");
	config->cantidad_entradas_tlb = config_get_int_value(archivo_config, "CANTIDAD_ENTRADAS_TLB");
	config->ip_memoria = config_get_string_value(archivo_config, "IP_MEMORIA");
	config->puerto_escucha_dispatch = config_get_int_value(archivo_config, "PUERTO_ESCUCHA_DISPATCH");
	config->puerto_escucha_interrupt = config_get_int_value(archivo_config, "PUERTO_ESCUCHA_INTERRUPT");
	config->puerto_memoria = config_get_int_value(archivo_config, "PUERTO_MEMORIA");

	return config;
}
