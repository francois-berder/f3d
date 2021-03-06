#ifndef F3D_TEXTURE_H
#define F3D_TEXTURE_H

#include <f3d/engine/image/image.h>

#include <GL/glew.h>
#include <GL/gl.h>

#define TEXTURE_TYPE_RGB     GL_RGB
#define TEXTURE_TYPE_RGBA    GL_RGBA

typedef struct {
    unsigned id;
    // type of data stored e.g RGB, RGBA, etc.
    int data_type;
    int draw_type;
    int bind_type;
    int index;
    
    image_t image;
} texture_t;

texture_t *texture_new(void);
void texture_init(texture_t *texture);
texture_t *texture_load(texture_t *texture, const char *path, int type);
texture_t *texture_load_data(texture_t *texture, const char *path, int type);
void texture_set_data(texture_t *texture, int width, int height, int type_size, unsigned char *data);
void texture_set_parameter(texture_t *texture, int parameter, int value);
void texture_destroy(texture_t *tex);
void textures_cleanup(void);

#endif
