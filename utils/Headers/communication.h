#ifndef HEADERS_COMMUNICATION_H_
#define HEADERS_COMMUNICATION_H_

#include "models.h"
#include "enums.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <commons/collections/list.h>

typedef enum handshake_status {
	HANDSHAKE_OK,
	HANDSHAKE_NO
} handshake_status;

void send_paquete(t_buffer* buffer, cod_op codigo_operacion, int socket);
t_paquete* recv_paquete(int socket);
int send_handshake(int socket);
int recv_handshake(int socket);

#endif /* HEADERS_COMMUNICATION_H_ */
