#include "../Headers/config.h"

void cargar_config(char* path) {
	t_config* archivo_config = config_create(path);
	app_config = malloc(sizeof(t_io_config));

	app_config->block_count = config_get_int_value(archivo_config, "BLOCK_COUNT");
	app_config->block_size = config_get_int_value(archivo_config, "BLOCK_SIZE");
	app_config->ip_kernel = config_get_string_value(archivo_config, "IP_KERNEL");
	app_config->ip_memoria = config_get_string_value(archivo_config, "IP_MEMORIA");
	app_config->path_base_dialfs = config_get_string_value(archivo_config, "PATH_BASE_DIALFS");
	app_config->puerto_kernel = config_get_int_value(archivo_config, "PUERTO_KERNEL");
	app_config->puerto_memoria = config_get_int_value(archivo_config, "PUERTO_MEMORIA");
	app_config->tiempo_unidad_trabajo = config_get_int_value(archivo_config, "TIEMPO_UNIDAD_TRABAJO") / 1000;
	app_config->tipo_infertaz = config_get_string_value(archivo_config, "TIPO_INTERFAZ");
}
