#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config("app.config");
	iniciar_log("app.log");

	espacio_memoria = malloc(app_config->tam_memoria);
	tabla_paginas = list_create();

	size_t tamanio_en_byte = (app_config->tam_memoria/app_config->tam_pagina)/8;
	bits_memoria = malloc(tamanio_en_byte);
	bitarray_memoria = bitarray_create_with_mode(bits_memoria, tamanio_en_byte, MSB_FIRST);

	instrucciones_procesos = list_create();
	iniciar_servidor();

	return 0;
}
