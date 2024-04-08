#include "../Headers/config.h"

t_memoria_config* cargar_config(char* path) {
	t_config* archivo_config = config_create(path);
	t_memoria_config* config = malloc(sizeof(t_memoria_config));

	config->path_instrucciones = config_get_string_value(archivo_config, "PATH_INSTRUCCIONES");
	config->puerto_escucha = config_get_int_value(archivo_config, "PUERTO_ESCUCHA");
	config->retardo_respuesta = config_get_int_value(archivo_config, "RETARDO_RESPUESTA");
	config->tam_memoria = config_get_int_value(archivo_config, "TAM_MEMORIA");
	config->tam_pagina = config_get_int_value(archivo_config, "TAM_PAGINA");

	return config;
}

void finalizar_config(t_memoria_config* config) {
	free(config->path_instrucciones);
	free(config);
}
