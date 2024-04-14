#include "../Headers/log.h"

void iniciar_log(char* path) {
	app_log = log_create(path, "IO", false, LOG_LEVEL_INFO);

	if(app_log == NULL)
		abort();
}
