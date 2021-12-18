#include <stdio.h>
#include <ctime>
#include <iostream>
#include <obs-module.h>
#include <graphics/image-file.h>
#include "image-manipulation.hpp"

struct image_source {
	obs_source_t *source;

	char *file;
	bool persistent;
	bool linear_alpha;
	time_t file_timestamp;
	float update_time_elapsed;
	uint64_t last_time;
	bool active;
	bool restart_gif;

	gs_image_file3_t if3;
};

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("image-manipulation", "en-GB")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Image manipulation source";
}