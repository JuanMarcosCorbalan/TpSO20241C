#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);

	iniciar_log("app.log");

	iniciar_conexiones();

	iniciar_interfaces(argv[2]);

	if(!strcmp(app_config->tipo_infertaz, "DIALFS")){
		iniciar_filesystem();
	}
	return 0;
}
