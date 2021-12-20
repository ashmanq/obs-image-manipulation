#include "image-manipulation-source.hpp"

#define blog(log_level, format, ...)                \
	blog(log_level, "[image_source: '%s'] " format, \
		 obs_source_get_name(context->source), ##__VA_ARGS__)

#define debug(format, ...) blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) blog(LOG_WARNING, format, ##__VA_ARGS__)

namespace IM
{
	// Image_Manipulation_Source::Image_Manipulation_Source(obs_source_t *new_source, obs_data_t *settings)
	// {
	// 	custom_source.source = new_source;
	// 	custom_source.settings = settings;
	// };

	// time_t Image_Manipulation_Source::get_modified_timestamp(const char *filename)
	// {
	// 	struct stat stats;
	// 	if (os_stat(filename, &stats) != 0)
	// 		return -1;
	// 	return stats.st_mtime;
	// }

	static const char* Image_Manipulation_Source::image_source_get_name(void *unused)
	{
		UNUSED_PARAMETER(unused);
		return obs_module_text("Source");
	}


	// static void Image_Manipulation_Source::image_source_create(obs_data_t *settings, obs_source_t *source)
	// {
	// 	CUSTOM_IMAGE_SOURCE *context = (CUSTOM_IMAGE_SOURCE *)bzalloc(sizeof(CUSTOM_IMAGE_SOURCE));
	// 	context->source = source;

	// 	image_source_update(context, settings);
	// 	return context;
	// }

	// static void image_source_destroy(void *data)
	// {
	// 	struct image_source *context = data;

	// 	image_source_unload(context);

	// 	if (context->file)
	// 		bfree(context->file);
	// 	bfree(context);
	// }

	// void Image_Manipulation_Source::register_image_manipulation_source()
	// {
	// 	static struct obs_source_info image_manipulation_source_info = {
	// 		.id = "image_manipulation_source",
	// 		.type = OBS_SOURCE_TYPE_INPUT,
	// 		.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_SRGB,
	// 		.get_name = image_source_get_name,
	// 		// .create = [](obs_data_t *settings, obs_source_t *source)s{ return static_cast<void *>(new Image_Manipulation_Source(source, settings)); },
	// 		// .destroy = image_source_destroy,
	// 		// .update = [](void *data, obs_data_t *settings){ reinterpret_cast<Image_Manipulation_Source *>(data)->image_source_update(data, settings); },
	// 		// .update =  <*>image_source_update,
	// 		// .get_defaults = image_source_defaults,
	// 		// .show = image_source_show,
	// 		// .hide = image_source_hide,
	// 		// .get_width = image_source_getwidth,
	// 		// .get_height = image_source_getheight,
	// 		// .video_render = image_source_render,
	// 		// .video_tick = image_source_tick,
	// 		// .missing_files = image_source_missingfiles,
	// 		// .get_properties = image_source_properties,
	// 		.icon_type = OBS_ICON_TYPE_IMAGE,
	// 		// .activate = image_source_activate,
	// 	};

	// 	obs_register_source(&image_manipulation_source_info);
	// }
}
