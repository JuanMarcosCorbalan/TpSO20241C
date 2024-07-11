#include "../Headers/config.h"

void cargar_config(char* path) {
	app_config_file = config_create(path);
	app_config = malloc(sizeof(t_kernel_config));

	app_config->algoritmo_planificacion = config_get_string_value(app_config_file, "ALGORITMO_PLANIFICACION");
	app_config->grado_multiprogramacion = config_get_int_value(app_config_file, "GRADO_MULTIPROGRAMACION");
	app_config->ip_cpu = config_get_string_value(app_config_file, "IP_CPU");
	app_config->ip_memoria = config_get_string_value(app_config_file, "IP_MEMORIA");
	app_config->puerto_cpu_dispatch = config_get_int_value(app_config_file, "PUERTO_CPU_DISPATCH");
	app_config->puerto_cpu_interrupt = config_get_int_value(app_config_file, "PUERTO_CPU_INTERRUPT");
	app_config->puerto_escucha = config_get_int_value(app_config_file, "PUERTO_ESCUCHA");
	app_config->puerto_memoria = config_get_int_value(app_config_file, "PUERTO_MEMORIA");
	app_config->quantum = config_get_int_value(app_config_file, "QUANTUM");
	app_config->recursos = list_create();

	char* recursos = config_get_string_value(app_config_file, "RECURSOS");
	char* recursos_init = string_replace(recursos, "[", "");
	char* recursos_fin = string_replace(recursos_init, "]", "");
	char** list_recursos = string_split(recursos_fin, ",");

	free(recursos);
	free(recursos_init);
	free(recursos_fin);

	char* instancias_recursos = config_get_string_value(app_config_file, "INSTANCIAS_RECURSOS");
	char* instancia_recursos_init = string_replace(instancias_recursos, "[", "");
	char* instancia_recurso_fin = string_replace(instancia_recursos_init, "]", "");
	char** list_instancias_recursos = string_split(instancia_recurso_fin, ",");

	free(instancias_recursos);
	free(instancia_recursos_init);
	free(instancia_recurso_fin);

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
		list_add(app_config->recursos, tad_recurso);

		free(recurso);
		free(cantidad_recurso);

		count_recursos++;
		recurso = list_recursos[count_recursos];
		cantidad_recurso = list_instancias_recursos[count_recursos];
	}
	free(list_recursos);
	free(list_instancias_recursos);
}

void finalizar_config() {
	free(app_config->algoritmo_planificacion);
	free(app_config->ip_cpu);
	free(app_config->ip_memoria);

	void liberar_recurso(void* elem) {
		t_recurso_sistema* recurso = elem;
		free(recurso->nombre);
		list_destroy(recurso->asignacion);
		list_destroy(recurso->solicitud);
		free(recurso);
	}

	list_destroy_and_destroy_elements(app_config->recursos ,liberar_recurso);
}
