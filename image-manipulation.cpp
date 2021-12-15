#include <stdio.h>
#include <ctime>
#include <iostream>
#include <obs-module.h>
#include <graphics/image-file.h>
#include "image-manipulation.h"

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

int main() {
    puts("Hello World!");
    printf("Version: %d.%d\n", image_manipulation_VERSION_MAJOR, image_manipulation_VERSION_MINOR);
}
