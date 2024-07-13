#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config(argv[1]);
	iniciar_log("app.log");

	existe_interrupcion = 0;
	motivo_interrupt_bloqueo = 0;
	motivo_interrupt_exit = 0;
	lista_tlb = list_create();

	iniciar_conexiones();

	finalizar_config();
	config_destroy(archivo_config);
	log_destroy(app_log);

	close(socket_memoria);
	close(socket_escucha_interrupt);
	close(socket_escucha_dispatch);

	return 0;
}
