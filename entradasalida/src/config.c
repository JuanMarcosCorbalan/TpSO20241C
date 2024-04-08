#include "../Headers/config.h"

t_io_config* cargar_config(char* path) {
	t_config* archivo_config = config_create(path);
	t_io_config* config = malloc(sizeof(t_io_config));

	config->block_count = config_get_int_value(archivo_config, "BLOCK_COUNT");
	config->block_size = config_get_int_value(archivo_config, "BLOCK_SIZE");
	config->ip_kernel = config_get_string_value(archivo_config, "IP_KERNEL");
	config->ip_memoria = config_get_string_value(archivo_config, "IP_MEMORIA");
	config->path_base_dialfs = config_get_string_value(archivo_config, "PATH_BASE_DIALFS");
	config->puerto_kernel = config_get_int_value(archivo_config, "PUERTO_KERNEL");
	config->puerto_memoria = config_get_int_value(archivo_config, "PUERTO_MEMORIA");
	config->tiempo_unidad_trabajo = config_get_int_value(archivo_config, "TIEMPO_UNIDAD_TRABAJO");
	config->tipo_infertaz = config_get_string_value(archivo_config, "TIPO_INTERFAZ");

	return config;
}
