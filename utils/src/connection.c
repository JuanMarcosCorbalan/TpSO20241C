#include "../Headers/connection.h"

int crear_socket_escucha(int puerto, int cantidad_conexiones) {
	struct addrinfo hints, *servinfo;
	char* str_puerto = string_itoa(puerto);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(NULL, str_puerto, &hints, &servinfo);

	int socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if(socket_servidor < 0){
		free(str_puerto);
		return -1;
	}

	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	listen(socket_servidor, cantidad_conexiones);
	freeaddrinfo(servinfo);
	free(str_puerto);

	return socket_servidor;
}

int crear_socket_cliente(int puerto, char* ip) {
	struct addrinfo hints;
	struct addrinfo *server_info;
	char* str_puerto = string_itoa(puerto);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(ip, str_puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,server_info->ai_socktype, server_info->ai_protocol);
	if(socket_cliente < 0){
		free(str_puerto);
		return -1;
	}

	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	freeaddrinfo(server_info);
	free(str_puerto);

	return socket_cliente;
}

int establecer_conexion_cliente(char* ip, int puerto) {
	int socket = crear_socket_cliente(puerto, ip);
	if(socket < 0)
		abort();

	int estado_comunicacion = send_handshake(socket);
	if(estado_comunicacion == 0)
		abort();

	return socket;
}
