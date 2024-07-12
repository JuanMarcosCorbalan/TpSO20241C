#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);
	iniciar_log("app.log");

	iniciar_espacio_memoria();

	instrucciones_procesos = list_create();
	iniciar_servidor();

	eliminar_instrucciones();
	log_destroy(app_log);
	config_destroy(archivo_config);

	return 0;
}
