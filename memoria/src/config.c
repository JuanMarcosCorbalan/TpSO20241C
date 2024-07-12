#include "../Headers/config.h"

void cargar_config(char* path) {
	archivo_config = config_create(path);
	app_config = malloc(sizeof(t_memoria_config));

	app_config->path_instrucciones = config_get_string_value(archivo_config, "PATH_INSTRUCCIONES");
	app_config->puerto_escucha = config_get_int_value(archivo_config, "PUERTO_ESCUCHA");
	app_config->retardo_respuesta = config_get_int_value(archivo_config, "RETARDO_RESPUESTA");
	app_config->tam_memoria = config_get_int_value(archivo_config, "TAM_MEMORIA");
	app_config->tam_pagina = config_get_int_value(archivo_config, "TAM_PAGINA");
}

void finalizar_config(t_memoria_config* config) {
	free(config->path_instrucciones);
	free(config);
}
