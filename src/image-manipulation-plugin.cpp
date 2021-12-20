#include <obs-module.h>
#include "./sources/image-manipulation-source.hpp"


OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("image-manipulation", "en-GB")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "Image manipulation source";
}

bool obs_module_load(void)
{
	IM::Image_Manipulation_Source new_image_source;
	new_image_source.register_image_manipulation_source();
	
	return true;
}