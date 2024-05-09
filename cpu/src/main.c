#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);
	iniciar_log("app.log");

	iniciar_conexiones();

	return 0;
}
