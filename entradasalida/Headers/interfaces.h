#ifndef HEADERS_INTERFACES_H_
#define HEADERS_INTERFACES_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "../../utils/Headers/mensajeria_kernel_io.h"
#include "../../utils/Headers/models.h"
#include "../../utils/Headers/communication.h"
#include <string.h>

extern t_io_config* app_config;
extern int socket_kernel;

void iniciar_interfaces(char* nombre);

#endif /* HEADERS_INTERFACES_H_ */
