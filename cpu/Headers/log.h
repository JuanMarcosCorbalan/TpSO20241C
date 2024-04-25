#ifndef HEADERS_LOG_H_
#define HEADERS_LOG_H_

#include <stdlib.h>
#include <commons/log.h>
#include <stdint.h>

extern t_log* app_log;

void iniciar_log(char* path);
void logear_fetch_instruccion(uint32_t pid, uint32_t program_counter);
void logear_instruccion_ejecutada(uint32_t pid, char* instruccion, char* parametros);
void logear_tlb_hit(uint32_t pid, uint32_t pagina);
void logear_tlb_miss(uint32_t pid, uint32_t pagina);
void logear_obtener_marco(uint32_t pid, uint32_t pagina, uint32_t marco);
void logear_escritura_memoria(uint32_t pid, uint32_t direccion, uint32_t valor);
void logear_lectura_memoria(uint32_t pid, uint32_t direccion, uint32_t valor);

#endif /* HEADERS_LOG_H_ */
