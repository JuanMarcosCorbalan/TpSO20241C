#ifndef HEADERS_ENUMS_H_
#define HEADERS_ENUMS_H_

typedef enum cod_op {
	HANDSKAHE,
	MSG_INICIAR_PROCESO,
	MSG_FINALIZAR_PROCESO,
	MSG_CONTEXTO_PROCESO,
	MSG_PROXIMA_INSTRUCCION,
	MSG_INSTRUCCION,
	MSG_INTERRUPT_BLOQUEAR,
	MSG_INTERRUPT_EXIT,
	MSG_IO_GEN_SLEEP,
	MSG_INICIAR_INTERFAZ,
	MSG_VALIDAR_INTERFAZ,
	MSG_ESTADO_VALIDAR_INTERFAZ,
	MSG_DESBLOQUEAR_IO,
	MSG_FINALIZAR_IO,
	MSG_DESALOJO,
	MSG_WAIT_RECURSO,
	MSG_SIGNAL_RECURSO,
	MSG_DESBLOQUEAR_CPU,
	MSG_TAMANIO_PAGINA,
	MSG_SOLICITUD_TAMANIO_PAGINA,
	MSG_RESIZE_PROCESO,
	MSG_STATUS_RESIZE_PROCESO,
	MSG_MARCO_PAGINA,
	MSG_IO_STDOUT_WRITE,
	MSG_IO_STDIN_READ,
	MSG_MOV_OUT,
	MSG_MOV_IN
} cod_op;

typedef enum tipo_interfaz {
	GENERICA,
	STDOUT,
	STDIN,
	DIALFS,
	I_ERRONEA
} tipo_interfaz;

typedef enum algoritmo_planificacion {
	FIFO,
	RR,
	VRR
} algoritmo_planificacion;

typedef enum tipo_instruccion {
	SET,
	MOV_IN,
	MOV_OUT,
	SUM,
	SUB,
	JNZ,
	RESIZE,
	COPY_STRING,
	WAIT,
	SIGNAL,
	IO_GEN_SLEEP,
	IO_STDIN_READ,
	IO_STDOUT_WRITE,
	IO_FS_CREATE,
	IO_FS_DELETE,
	IO_FS_TRUNCATE,
	IO_FS_WRITE,
	IO_FS_READ,
	EXIT
} tipo_instruccion;

typedef enum motivo_exit {
	SIN_MOTIVO_EXIT,
	SUCCESS,
	INVALID_RESOURCE,
	INVALID_WRITE
} motivo_exit;

typedef enum motivo_blocked {
	SIN_MOTIVO_BLOCKED,
	INTERFAZ,
	RECURSO,
	OUT_OF_MEMORY
} motivo_blocked;

typedef enum estado_proceso {
	NEW,
	READY,
	V_READY,
	BLOCKED,
	EXEC,
	_EXIT
} estado_proceso;

#endif /* HEADERS_ENUMS_H_ */
