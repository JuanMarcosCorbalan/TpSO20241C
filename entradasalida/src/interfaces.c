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
		pthread_mutex_lock(&mutex_kernel);

		t_paquete* paquete = recv_paquete(socket_kernel);
		int operacion_valida = 0;
		char* nombre_instruccion;
		dt_io_sleep* io_sleep;
		dt_io_std* io_std;
		char* linea_consola;
		char* sub_linea_consola;
		uint32_t estado_escritura;
		dt_fs_name* fs_name;
		dt_fs_rw* fs_rw;
		dt_fs_truncate* fs_truncate;
		char* valor_escritura;
		void* void_escritura;

		switch(paquete->codigo_operacion) {
			case MSG_VALIDAR_INTERFAZ:
				nombre_instruccion = deserializar_validar_interfaz(paquete->buffer);
				operacion_valida = validar_tipo_instruccion(tipo_interfaz_actual, nombre_instruccion);
				request_estado_validacion_instruccion(socket_kernel, operacion_valida);
				free(nombre_instruccion);
				break;
			case MSG_IO_GEN_SLEEP:
				io_sleep = deserializar_io_gen_sleep(paquete->buffer);
				logear_operacion(io_sleep->pid, paquete->codigo_operacion);
				usleep((app_config->tiempo_unidad_trabajo * io_sleep->unidad_trabajo) * 1000);
				request_desbloquear_proceso_io(socket_kernel, io_sleep->pid);
				free(io_sleep->instruccion);
				free(io_sleep);
				break;
			case MSG_IO_STDIN_READ:
				io_std = deserializar_io_std(paquete->buffer);
				logear_operacion(io_std->pid, paquete->codigo_operacion);
				printf("Ingrese una cadena de caracteres para guardar en memoria\n");
				linea_consola = readline(">");
				sub_linea_consola = string_substring_until(linea_consola, io_std->tamanio + 1);
				request_escritura_memoria(socket_memoria, io_std->pid, io_std->direccion_fisica, sub_linea_consola);

				request_proceso_io_esperando(socket_kernel, io_std->pid);

				estado_escritura = deserializar_status_escritura_memoria(socket_memoria);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				if(estado_escritura)
					request_desbloquear_proceso_io(socket_kernel, io_std->pid);
				else
					request_finalizar_proceso_io(socket_kernel, io_std->pid);
				free(sub_linea_consola);
				free(linea_consola);
				free(io_std);
				break;
			case MSG_IO_STDOUT_WRITE:
				io_std = deserializar_io_std(paquete->buffer);
				logear_operacion(io_std->pid, paquete->codigo_operacion);
				request_lectura_memoria(socket_memoria, io_std->pid, io_std->direccion_fisica, io_std->tamanio + 1);
				linea_consola = deserializar_resultado_lectura_memoria(socket_memoria);

				request_proceso_io_esperando(socket_kernel, io_std->pid);

				printf("La cadena de caracteres obtenida de memoria es: %s \n", linea_consola);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				request_desbloquear_proceso_io(socket_kernel, io_std->pid);
				free(linea_consola);
				free(io_std);
				break;
			case MSG_IO_FS_CREATE:
				fs_name = deserializar_fs_name(paquete->buffer);
				logear_operacion(fs_name->pid, paquete->codigo_operacion);
				create(fs_name->pid, fs_name->nombre_archivo);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				request_desbloquear_proceso_io(socket_kernel, fs_name->pid);
				free(fs_name->nombre_archivo);
				free(fs_name);
				break;
			case MSG_IO_FS_DELETE:
				fs_name = deserializar_fs_name(paquete->buffer);
				logear_operacion(fs_name->pid, paquete->codigo_operacion);
				delete(fs_name->pid, fs_name->nombre_archivo);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				request_desbloquear_proceso_io(socket_kernel, fs_name->pid);
				free(fs_name->nombre_archivo);
				free(fs_name);
				break;
			case MSG_IO_FS_TRUNCATE:
				fs_truncate = deserializar_truncate_archivo(paquete->buffer);
				logear_operacion(fs_truncate->fs_name->pid, paquete->codigo_operacion);
				truncar(fs_truncate->fs_name->pid, fs_truncate->fs_name->nombre_archivo, fs_truncate->tamanio);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				request_desbloquear_proceso_io(socket_kernel, fs_truncate->fs_name->pid);
				free(fs_truncate->fs_name->nombre_archivo);
				free(fs_truncate->fs_name);
				free(fs_truncate);
				break;
			case MSG_IO_FS_READ:
				fs_rw = deserializar_fs_rw(paquete->buffer);
				logear_operacion(fs_rw->fs_name->pid, paquete->codigo_operacion);
				void_escritura = read_fs(fs_rw->fs_name->pid, fs_rw->fs_name->nombre_archivo, fs_rw->registro_tamanio, fs_rw->registro_puntero_archivo);
				valor_escritura = malloc(fs_rw->registro_tamanio);
				memcpy(valor_escritura, void_escritura, fs_rw->registro_tamanio);
				request_fs_escritura(socket_memoria, fs_rw->fs_name->pid, fs_rw->registro_direccion, valor_escritura);
				deserializar_status_escritura_memoria(socket_memoria);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				request_desbloquear_proceso_io(socket_kernel, fs_rw->fs_name->pid);
				free(valor_escritura);
				free(void_escritura);
				free(fs_rw->fs_name->nombre_archivo);
				free(fs_rw->fs_name);
				free(fs_rw);
				break;
			case MSG_IO_FS_WRITE:
				fs_rw = deserializar_fs_rw(paquete->buffer);
				logear_operacion(fs_rw->fs_name->pid, paquete->codigo_operacion);
				request_fs_lectura(socket_memoria, fs_rw->fs_name->pid, fs_rw->registro_direccion, fs_rw->registro_tamanio);
				valor_escritura = deserializar_valor_fs_lectura(socket_memoria);
				void_escritura = malloc(fs_rw->registro_tamanio);
				memcpy(void_escritura, valor_escritura, fs_rw->registro_tamanio);
				write_fs(fs_rw->fs_name->pid, fs_rw->fs_name->nombre_archivo, fs_rw->registro_tamanio, fs_rw->registro_puntero_archivo, void_escritura);
				usleep(app_config->tiempo_unidad_trabajo * 1000);
				request_desbloquear_proceso_io(socket_kernel, fs_rw->fs_name->pid);
				free(void_escritura);
				free(valor_escritura);
				free(fs_rw->fs_name->nombre_archivo);
				free(fs_rw->fs_name);
				free(fs_rw);
				break;
			default:
				seguir_operando = 0;
				break;
		}

		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);

		pthread_mutex_unlock(&mutex_kernel);
	}
}

void iniciar_interfaces(char* nombre) {
	pthread_mutex_init(&mutex_kernel, NULL);
	request_iniciar_interfaz(socket_kernel, nombre);
	operar_kernel();
}
