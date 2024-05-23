#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config("app.config");
	iniciar_log("app.log");

	iniciar_espacio_memoria();

	instrucciones_procesos = list_create();
	iniciar_servidor();

	return 0;
}
