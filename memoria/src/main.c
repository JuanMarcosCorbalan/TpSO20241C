#include "../Headers/main.h"

int main(int argc, char *argv[]) {
	cargar_config("app.config");
	iniciar_log("app.log");

	espacio_memoria = malloc(app_config->tam_memoria);
	tabla_paginas = list_create();

	size_t tamanio_en_byte = (app_config->tam_memoria/app_config->tam_pagina);
	puntero_bitmap = malloc(tamanio_en_byte);
	bitarray_memoria = bitarray_create_with_mode(puntero_bitmap, tamanio_en_byte, LSB_FIRST);

	int bits = bitarray_get_max_bit(bitarray_memoria);
    for (int c=0; c<bits; c++)
    {
        bitarray_clean_bit(bitarray_memoria, c);
    }

	instrucciones_procesos = list_create();
	iniciar_servidor();

	return 0;
}
