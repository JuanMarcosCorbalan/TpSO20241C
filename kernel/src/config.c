#include "../Headers/config.h"

t_kernel_config* cargar_config(char* path) {
	t_config* archivo_config = config_create(path);
	t_kernel_config* config = malloc(sizeof(t_kernel_config));

	config->algoritmo_planificacion = config_get_string_value(archivo_config, "ALGORITMO_PLANIFICACION");
	config->grado_multiprogramacion = config_get_int_value(archivo_config, "GRADO_MULTIPROGRAMACION");
	config->ip_cpu = config_get_string_value(archivo_config, "IP_CPU");
	config->ip_memoria = config_get_string_value(archivo_config, "IP_MEMORIA");
	config->puerto_cpu_dispatch = config_get_int_value(archivo_config, "PUERTO_CPU_DISPATCH");
	config->puerto_cpu_interrupt = config_get_int_value(archivo_config, "PUERTO_CPU_INTERRUPT");
	config->puerto_escucha = config_get_int_value(archivo_config, "PUERTO_ESCUCHA");
	config->puerto_memoria = config_get_int_value(archivo_config, "PUERTO_MEMORIA");
	config->quantum = config_get_int_value(archivo_config, "QUANTUM");
	config->recursos = list_create();

	char* recursos = config_get_string_value(archivo_config, "RECURSOS");
	recursos = string_replace(recursos, "[", "");
	recursos = string_replace(recursos, "]", "");
	char** list_recursos = string_split(recursos, ",");
	free(recursos);

	char* instancias_recursos = config_get_string_value(archivo_config, "INSTANCIAS_RECURSOS");
	instancias_recursos = string_replace(instancias_recursos, "[", "");
	instancias_recursos = string_replace(instancias_recursos, "]", "");
	char** list_instancias_recursos = string_split(instancias_recursos, ",");
	free(instancias_recursos);

	int count_recursos = 0;
	char* recurso = list_recursos[count_recursos];
	char* cantidad_recurso = list_instancias_recursos[count_recursos];

	while(recurso != NULL) {
		t_recurso_sistema* tad_recurso = malloc(sizeof(t_recurso_sistema));
		tad_recurso->solicitud = list_create();
		tad_recurso->asignacion = list_create();
		tad_recurso->nombre = malloc(strlen(recurso)+ 1);
		memcpy(tad_recurso->nombre, recurso, strlen(recurso)+ 1);
		tad_recurso->cantidad = atoi(cantidad_recurso);
		list_add(config->recursos, tad_recurso);

		free(recurso);
		free(cantidad_recurso);

		count_recursos++;
		recurso = list_recursos[count_recursos];
		cantidad_recurso = list_instancias_recursos[count_recursos];
	}

	free(list_recursos);
	free(list_instancias_recursos);

	return config;
}
