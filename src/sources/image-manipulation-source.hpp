#ifndef image_manipulation_source_h
#define image_manipulation_source_h

#include <obs-module.h>
#include <graphics/image-file.h>
#include <util/platform.h>
#include <util/dstr.h>
#include <sys/stat.h>
#include <cstdlib>

namespace IM
{
    class Image_Manipulation_Source
    {
        typedef struct image_source
        {
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
        } image_source;
    
    static const char *image_filter;

    public:
        // Image_Manipulation_Source(obs_source_t *source, obs_data_t *settings);
        void register_image_manipulation_source(void);

    private:
        static time_t get_modified_timestamp(const char *filename);
        static const char *image_source_get_name(void *);
        static void image_source_load(struct image_source *context);
        static void image_source_unload(struct image_source *context);
        static void image_source_update(void *data, obs_data_t *settings);
        static void image_source_defaults(obs_data_t *settings);
        static void image_source_show(void *data);
        static void image_source_hide(void *data);
        static void restart_gif(void *data);
        static void image_source_activate(void *data);
        static void *image_source_create(obs_data_t *settings, obs_source_t *source);
        static void image_source_destroy(void *data);
        static uint32_t image_source_getwidth(void *data);
        static uint32_t image_source_getheight(void *data);
        static void image_source_render(void *data, gs_effect_t *effect);
        static void image_source_tick(void *data, float seconds);
        static obs_properties_t *image_source_properties(void *data);
        uint64_t image_source_get_memory_usage(void *data);
        static void missing_file_callback(void *src, const char *new_path, void *data);
        static obs_missing_files_t *image_source_missingfiles(void *data);
    };

}

#endif