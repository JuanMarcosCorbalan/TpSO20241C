#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config("app.config");
	iniciar_log("app.log");

	existe_interrupcion = 0;
	motivo_interrupt_bloqueo = 0;
	motivo_interrupt_exit = 0;

	iniciar_conexiones();

	return 0;
}
