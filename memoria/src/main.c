#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	app_config = cargar_config("app.config");
	app_log = iniciar_log("app.log");

	iniciar_servidor();

	return 0;
}
