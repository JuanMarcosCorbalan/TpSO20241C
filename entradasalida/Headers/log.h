#ifndef HEADERS_LOG_H_
#define HEADERS_LOG_H_

#include <stdlib.h>
#include <commons/log.h>
#include <stdint.h>
#include "../../utils/Headers/enums.h"

extern t_log* app_log;

void iniciar_log(char* path);
void logear_operacion(uint32_t pid, uint32_t operacion);
void logear_creacion_archivo(uint32_t pid, char* nombre);
void logear_eliminar_archivo(uint32_t pid, char* nombre);

#endif /* HEADERS_LOG_H_ */
