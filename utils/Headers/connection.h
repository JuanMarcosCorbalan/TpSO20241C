#ifndef HEADERS_CONNECTION_H_
#define HEADERS_CONNECTION_H_

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <commons/log.h>
#include <commons/string.h>
#include "../Headers/communication.h"

int crear_socket_escucha(int puerto, int cantidad_conexiones);
int crear_socket_cliente(int puerto, char* ip);
int establecer_conexion_cliente(char* ip, int puerto);

#endif /* HEADERS_CONNECTION_H_ */
