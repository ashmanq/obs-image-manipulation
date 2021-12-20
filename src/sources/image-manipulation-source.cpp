#include "image-manipulation-source.hpp"

#define blog(log_level, format, ...)                \
	blog(log_level, "[image_source: '%s'] " format, \
		 obs_source_get_name(context->source), ##__VA_ARGS__)

#define debug(format, ...) blog(LOG_DEBUG, format, ##__VA_ARGS__)
#define info(format, ...) blog(LOG_INFO, format, ##__VA_ARGS__)
#define warn(format, ...) blog(LOG_WARNING, format, ##__VA_ARGS__)

namespace IM
{
	time_t Image_Manipulation_Source::get_modified_timestamp(const char *filename)
	{
		struct stat stats;
		if (os_stat(filename, &stats) != 0)
			return -1;
		return stats.st_mtime;
	}

	const char *Image_Manipulation_Source::image_source_get_name(void *unused)
	{
		UNUSED_PARAMETER(unused);
		return obs_module_text("Source");
	}

	void Image_Manipulation_Source::image_source_load(struct image_source *context)
	{
		char *file = context->file;

		obs_enter_graphics();
		gs_image_file3_free(&context->if3);
		obs_leave_graphics();

		if (file && *file)
		{
			debug("loading texture '%s'", file);
			context->file_timestamp = Image_Manipulation_Source::get_modified_timestamp(file);
			gs_image_file3_init(&context->if3, file,
								context->linear_alpha
									? GS_IMAGE_ALPHA_PREMULTIPLY_SRGB
									: GS_IMAGE_ALPHA_PREMULTIPLY);
			context->update_time_elapsed = 0;

			obs_enter_graphics();
			gs_image_file3_init_texture(&context->if3);
			obs_leave_graphics();

			if (!context->if3.image2.image.loaded)
				warn("failed to load texture '%s'", file);
		}
	}

	void Image_Manipulation_Source::image_source_unload(struct image_source *context)
	{
		obs_enter_graphics();
		gs_image_file3_free(&context->if3);
		obs_leave_graphics();
	}

	void Image_Manipulation_Source::image_source_update(void *data, obs_data_t *settings)
	{

		image_source *context = static_cast<image_source*>(data);
		const char *file = obs_data_get_string(settings, "file");
		const bool unload = obs_data_get_bool(settings, "unload");
		const bool linear_alpha = obs_data_get_bool(settings, "linear_alpha");

		if (context->file)
			bfree(context->file);
		context->file = bstrdup(file);
		context->persistent = !unload;
		context->linear_alpha = linear_alpha;

		/* Load the image if the source is persistent or showing */
		if (context->persistent || obs_source_showing(context->source))
			Image_Manipulation_Source::image_source_load(static_cast<image_source*>(data));
		else
			Image_Manipulation_Source::image_source_unload(static_cast<image_source*>(data));
	}

	void Image_Manipulation_Source::image_source_defaults(obs_data_t *settings)
	{
		obs_data_set_default_bool(settings, "unload", false);
		obs_data_set_default_bool(settings, "linear_alpha", false);
	}

	void Image_Manipulation_Source::image_source_show(void *data)
	{
		struct image_source *context = static_cast<image_source*>(data);

		if (!context->persistent)
			image_source_load(context);
	}

	void Image_Manipulation_Source::image_source_hide(void *data)
	{
		image_source *context = static_cast<image_source*>(data);

		if (!context->persistent)
			image_source_unload(context);
	}

	void Image_Manipulation_Source::restart_gif(void *data)
	{
		image_source *context = static_cast<image_source*>(data);

		if (context->if3.image2.image.is_animated_gif) {
			context->if3.image2.image.cur_frame = 0;
			context->if3.image2.image.cur_loop = 0;
			context->if3.image2.image.cur_time = 0;

			obs_enter_graphics();
			gs_image_file3_update_texture(&context->if3);
			obs_leave_graphics();

			context->restart_gif = false;
		}
	}

	void Image_Manipulation_Source::image_source_activate(void *data)
	{
		struct image_source *context = static_cast<image_source*>(data);
		context->restart_gif = true;
	}

	void* Image_Manipulation_Source::image_source_create(obs_data_t *settings, obs_source_t *source)
	{
		Image_Manipulation_Source::image_source *context = static_cast<image_source*>(bzalloc(sizeof(Image_Manipulation_Source::image_source)));
		context->source = source;

		image_source_update(context, settings);
		return context;
	}

	void Image_Manipulation_Source::image_source_destroy(void *data)
	{
		struct image_source *context = static_cast<image_source*>(data);

		image_source_unload(context);

		if (context->file)
			bfree(context->file);
		bfree(context);
	}

	uint32_t Image_Manipulation_Source::image_source_getwidth(void *data)
	{
		struct image_source *context = static_cast<image_source*>(data);
		return context->if3.image2.image.cx;
	}

	uint32_t Image_Manipulation_Source::image_source_getheight(void *data)
	{
		struct image_source *context = static_cast<image_source*>(data);
		return context->if3.image2.image.cy;
	}

	void Image_Manipulation_Source::image_source_render(void *data, gs_effect_t *effect)
	{
		struct image_source *context = static_cast<image_source*>(data);

		if (!context->if3.image2.image.texture)
			return;

		const bool previous = gs_framebuffer_srgb_enabled();
		gs_enable_framebuffer_srgb(true);

		gs_blend_state_push();
		gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

		gs_eparam_t *const param = gs_effect_get_param_by_name(effect, "image");
		gs_effect_set_texture_srgb(param, context->if3.image2.image.texture);

		gs_draw_sprite(context->if3.image2.image.texture, 0,
				context->if3.image2.image.cx,
				context->if3.image2.image.cy);

		gs_blend_state_pop();

		gs_enable_framebuffer_srgb(previous);
	}

	void Image_Manipulation_Source::image_source_tick(void *data, float seconds)
	{
		struct image_source *context = static_cast<image_source*>(data);
		uint64_t frame_time = obs_get_video_frame_time();

		context->update_time_elapsed += seconds;

		if (obs_source_showing(context->source)) {
			if (context->update_time_elapsed >= 1.0f) {
				time_t t = get_modified_timestamp(context->file);
				context->update_time_elapsed = 0.0f;

				if (context->file_timestamp != t) {
					image_source_load(context);
				}
			}
		}

		if (obs_source_showing(context->source)) {
			if (!context->active) {
				if (context->if3.image2.image.is_animated_gif)
					context->last_time = frame_time;
				context->active = true;
			}

			if (context->restart_gif)
				restart_gif(context);

		} else {
			if (context->active) {
				restart_gif(context);
				context->active = false;
			}

			return;
		}

		if (context->last_time && context->if3.image2.image.is_animated_gif) {
			uint64_t elapsed = frame_time - context->last_time;
			bool updated = gs_image_file3_tick(&context->if3, elapsed);

			if (updated) {
				obs_enter_graphics();
				gs_image_file3_update_texture(&context->if3);
				obs_leave_graphics();
			}
		}

		context->last_time = frame_time;
	}
	
	const char* Image_Manipulation_Source::image_filter =
	"All formats (*.bmp *.tga *.png *.jpeg *.jpg *.gif *.psd *.webp);;"
	"BMP Files (*.bmp);;"
	"Targa Files (*.tga);;"
	"PNG Files (*.png);;"
	"JPEG Files (*.jpeg *.jpg);;"
	"GIF Files (*.gif);;"
	"PSD Files (*.psd);;"
	"WebP Files (*.webp);;"
	"All Files (*.*)";
	
	obs_properties_t* Image_Manipulation_Source::image_source_properties(void *data)
	{
		struct image_source *s = static_cast<image_source*>(data);
		struct dstr path = {0};

		obs_properties_t *props = obs_properties_create();

		if (s && s->file && *s->file) {
			const char *slash;

			dstr_copy(&path, s->file);
			dstr_replace(&path, "\\", "/");
			slash = strrchr(path.array, '/');
			if (slash)
				dstr_resize(&path, slash - path.array + 1);
		}

		obs_properties_add_path(props, "file", obs_module_text("File"),
					OBS_PATH_FILE, image_filter, path.array);
		obs_properties_add_bool(props, "unload",
					obs_module_text("UnloadWhenNotShowing"));
		obs_properties_add_bool(props, "linear_alpha",
					obs_module_text("LinearAlpha"));
		dstr_free(&path);

		return props;
	}

	uint64_t Image_Manipulation_Source::image_source_get_memory_usage(void *data)
	{
		struct image_source *s = static_cast<image_source*>(data);
		return s->if3.image2.mem_usage;
	}

	void Image_Manipulation_Source::missing_file_callback(void *src, const char *new_path, void *data)
	{
		struct image_source *s = static_cast<image_source*>(src);

		obs_source_t *source = s->source;
		obs_data_t *settings = obs_source_get_settings(source);
		obs_data_set_string(settings, "file", new_path);
		obs_source_update(source, settings);
		obs_data_release(settings);

		UNUSED_PARAMETER(data);
	}

	obs_missing_files_t* Image_Manipulation_Source::image_source_missingfiles(void *data)
	{
		struct image_source *s = static_cast<image_source*>(data);
		obs_missing_files_t *files = obs_missing_files_create();

		if (strcmp(s->file, "") != 0) {
			if (!os_file_exists(s->file)) {
				obs_missing_file_t *file = obs_missing_file_create(
					s->file, missing_file_callback,
					OBS_MISSING_FILE_SOURCE, s->source, NULL);

				obs_missing_files_add_file(files, file);
			}
		}

		return files;
	}

	void Image_Manipulation_Source::register_image_manipulation_source()
	{
		static struct obs_source_info image_manipulation_source_info = {
			.id = "image_manipulation_source",
			.type = OBS_SOURCE_TYPE_INPUT,
			.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_SRGB,
			.get_name = image_source_get_name,
			.create = image_source_create,
			.destroy = image_source_destroy,
			.update =  image_source_update,
			.get_defaults = image_source_defaults,
			.show = image_source_show,
			.hide = image_source_hide,
			.get_width = image_source_getwidth,
			.get_height = image_source_getheight,
			.video_render = image_source_render,
			.video_tick = image_source_tick,
			.missing_files = image_source_missingfiles,
			.get_properties = image_source_properties,
			.icon_type = OBS_ICON_TYPE_IMAGE,
			.activate = image_source_activate,
		};

		obs_register_source(&image_manipulation_source_info);
	}
}
