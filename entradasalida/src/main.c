#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);

	iniciar_log("app.log");

	if(!strcmp(app_config->tipo_infertaz, "DIALFS")){
		iniciar_filesystem();
	}

	iniciar_conexiones();

	iniciar_interfaces(argv[2]);

	log_destroy(app_log);
	finalizar_estructuras();
	finalizar_config();
	config_destroy(archivo_config);

	close(socket_kernel);
	close(socket_memoria);

	return 0;
}
