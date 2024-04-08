#include "../Headers/conexiones.h"

void operar(int *socket_cliente) {
	close(*socket_cliente);
	free(socket_cliente);
}

void iniciar_servidor() {
	int socket_escucha = crear_socket_escucha(app_config->puerto_escucha, 3);

	log_info(app_log, "Se crea conexión para la escucha con el socket id %d", socket_escucha);

	while(1) {
		int *socket_cliente = malloc(sizeof(int));
		*socket_cliente = accept(socket_escucha, NULL, NULL);

		if(*socket_cliente < 0)
			abort();

		log_info(app_log, "El cliente establece conexión con el socket id %d", *socket_cliente);

		int estado_comunicacion = recv_handshake(*socket_cliente);
		if(estado_comunicacion == 0)
			break;

		pthread_t hilo_cliente;
		pthread_create(&hilo_cliente, NULL, (void*) operar, socket_cliente);
		pthread_detach(hilo_cliente);
	}
}
