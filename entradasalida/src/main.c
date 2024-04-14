#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config("app.config");
//	cargar_config(argv[1]);

	iniciar_log("app.log");

	iniciar_conexiones();

	iniciar_interfaces("DISCO");
//	iniciar_interfaces(argv[0]);

	return 0;
}
