#include "../Headers/log.h"

t_log* iniciar_log(char* path) {
	t_log* app_log = log_create(path, "Memoria", false, LOG_LEVEL_INFO);

	if(app_log == NULL)
		abort();

	return app_log;
}
