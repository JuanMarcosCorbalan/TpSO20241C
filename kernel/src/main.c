#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);
	iniciar_log("app.log");

	iniciar_estructuras();
	iniciar_conexiones();

	iniciar_planificador_largo_plazo();
	iniciar_planificador_corto_plazo();

	iniciar_consola();

	finalizar_procesos();
	finalizar_estructuras();
	finalizar_config();
	log_destroy(app_log);
	config_destroy(app_config_file);

	close(socket_cpu_dispatch);
	close(socket_cpu_interrupt);
	close(socket_memoria);
	close(socket_escucha_io);

	return 0;
}
