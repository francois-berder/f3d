#include <f3d/engine/image/image.h>
#include <f3d/engine/image/bmp.h>
#include <stdlib.h>

void load_bmp(image_t *image, const char *path) {
    image_bmp_t *bmp = &(image->image.bmp);
    (*bmp) = bmp_load(path);
    if (bmp->data == NULL) {
        image->type = IMAGE_NONE;
        return;
    }
    image->type   = IMAGE_BMP;
    image->data   = bmp->data;
    image->width  = bmp->info.header.image_width;
    image->height = bmp->info.header.image_height;
    image->bpp    = bmp->info.header.pixel_bit_count;
}

image_t image_load(const char *path, int type) {
    image_t image;
    image.flags = 0;
    image.type = IMAGE_NONE;
    image.bpp = 0;
    switch (type) {
        case IMAGE_BMP:
            load_bmp(&image, path);
            break;
    };
    return image;
}

void image_destroy(image_t *image) {
    if (image->data != NULL || image->type == IMAGE_NONE)
        free(image->data);
        
    image->data = NULL;
    image->type = IMAGE_NONE;
}
