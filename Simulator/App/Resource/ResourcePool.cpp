#include "ResourcePool.h"
#include "Utils/ResourceManager/ResourceManager.h"
#include "lvgl/src/widgets/lv_img.h"

static ResourceManager Font_;
static ResourceManager Image_;

extern "C" {
#define IMPORT_FONT(name) \
do{\
    LV_FONT_DECLARE(font_##name)\
    Font_.AddResource(#name, (void*)&font_##name);\
}while(0)

#define IMPORT_IMG(name) \
do{\
    LV_IMG_DECLARE(img_src_##name)\
    Image_.AddResource(#name, (void*)&img_src_##name);\
}while (0)

    static void Resource_Init()
    {
        /* Import Fonts */
        IMPORT_FONT(oswaldBold_12);
        IMPORT_FONT(oswaldBold_18);
        IMPORT_FONT(rajdhaniBold_20);
        IMPORT_FONT(rajdhaniBold_40);
        // /* Import Images */
        IMPORT_IMG(battery);
        IMPORT_IMG(location);
        IMPORT_IMG(menu);
        IMPORT_IMG(pause);
        IMPORT_IMG(start);
    }

} /* extern "C" */

void ResourcePool::Init()
{
    Resource_Init();
    Font_.SetDefault((void*)LV_FONT_DEFAULT);
}

lv_font_t* ResourcePool::GetFont(const char* name)
{
    return static_cast<lv_font_t *>(Font_.GetResource(name));
}
const void* ResourcePool::GetImage(const char* name)
{
    return Image_.GetResource(name);
}
