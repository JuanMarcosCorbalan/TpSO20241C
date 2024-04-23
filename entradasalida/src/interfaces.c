#include "../Headers/interfaces.h"

uint8_t obtener_tipo_interfaz() {
	if(strcmp(app_config->tipo_infertaz, "GENERICA") == 0)
		return GENERICA;
	else if(strcmp(app_config->tipo_infertaz, "STDOUT") == 0)
		return STDOUT;
	else if(strcmp(app_config->tipo_infertaz, "STDIN") == 0)
		return STDIN;
	else if(strcmp(app_config->tipo_infertaz, "DIALFS") == 0)
		return DIALFS;
	else
		return I_ERRONEA;
}

int validar_tipo_instruccion(uint8_t tipo_interfaz, char* instruccion) {
	if(tipo_interfaz == GENERICA && strcmp(instruccion, "IO_GEN_SLEEP") == 0)
		return 1;
	else if(tipo_interfaz == STDIN && strcmp(instruccion, "IO_STDIN_READ") == 0)
		return 1;
	else if(tipo_interfaz == STDOUT && strcmp(instruccion, "IO_STDOUT_WRITE") == 0)
		return 1;
	else if(tipo_interfaz == DIALFS) {
		if(strcmp(instruccion, "IO_FS_CREATE") == 0)
			return 1;
		else if(strcmp(instruccion, "IO_FS_DELETE") == 0)
			return 1;
		else if(strcmp(instruccion, "IO_FS_TRUNCATE") == 0)
			return 1;
		else if(strcmp(instruccion, "IO_FS_WRITE") == 0)
			return 1;
		else if(strcmp(instruccion, "IO_FS_READ") == 0)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void operar_kernel() {
	uint8_t tipo_interfaz_actual = obtener_tipo_interfaz();
	int seguir_operando = 1;

	while(seguir_operando) {

		t_paquete* paquete = recv_paquete(socket_kernel);
		int operacion_valida = 0;
		char* nombre_instruccion;
		dt_io_sleep* io_sleep;
		dt_io_std* io_std;

		switch(paquete->codigo_operacion) {
			case MSG_VALIDAR_INTERFAZ:
				nombre_instruccion = deserializar_validar_interfaz(paquete->buffer);
				operacion_valida = validar_tipo_instruccion(tipo_interfaz_actual, nombre_instruccion);
				request_estado_validacion_instruccion(socket_kernel, operacion_valida);
				free(nombre_instruccion);
				break;
			case MSG_IO_GEN_SLEEP:
				io_sleep = deserializar_io_gen_sleep(paquete->buffer);
				sleep((app_config->tiempo_unidad_trabajo * io_sleep->unidad_trabajo)/100);
				request_desbloquear_proceso(socket_kernel, io_sleep->pid);
				free(io_sleep->instruccion);
				free(io_sleep);
				break;
			case MSG_IO_STDIN_READ:
				io_std = deserializar_io_std(paquete->buffer);
				// ACÁ SE EJECUTA LA LÓGICA DEL STDIN_READ
				// UNA VEZ QUE TERMINE, TENEMOS QUE CONTINUAR CON EL MENSAJE DE DESBLOQUEAR_PROCESO
				sleep(app_config->tiempo_unidad_trabajo / 100);
				request_desbloquear_proceso(socket_kernel, io_std->pid);
				free(io_std);
				break;
			case MSG_IO_STDOUT_WRITE:
				io_std = deserializar_io_std(paquete->buffer);
				// ACÁ SE EJECUTA LA LÓGICA DEL STDOUT_READ
				// UNA VEZ QUE TERMINE, TENEMOS QUE CONTINUAR CON EL MENSAJE DE DESBLOQUEAR_PROCESO
				sleep(app_config->tiempo_unidad_trabajo / 100);
				request_desbloquear_proceso(socket_kernel, io_std->pid);
				free(io_std);
				break;
			default:
				seguir_operando = 0;
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);
	}
}

void iniciar_interfaces(char* nombre) {
	request_iniciar_interfaz(socket_kernel, nombre);
	operar_kernel();
}
