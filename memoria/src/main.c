#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);
	iniciar_log("app.log");

	instrucciones_procesos = list_create();
	iniciar_servidor();

	return 0;
}
