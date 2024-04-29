#ifndef HEADERS_LOG_H_
#define HEADERS_LOG_H_

#include <stdlib.h>
#include <commons/log.h>
#include <stdint.h>

extern t_log* app_log;

void iniciar_log(char* path);
void logear_tabla_paginas(uint32_t pid, uint32_t cantidad);
void logear_acceso_tabla_paginas(uint32_t pid, uint32_t pagina, uint32_t marco);
void logear_ampliacion_proceso(uint32_t pid, uint32_t tamanio_actual, uint32_t tamanio_nuevo);
void logear_reduccion_proceso(uint32_t pid, uint32_t tamanio_actual, uint32_t tamanio_nuevo);
void logear_lectura_espacio_usuario(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio);
void logear_escritura_espacio_usuario(uint32_t pid, uint32_t direccion_fisica, uint32_t tamanio);

#endif /* HEADERS_LOG_H_ */
