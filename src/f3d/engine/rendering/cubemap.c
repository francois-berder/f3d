#include <f3d/engine/rendering/cubemap.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/image/texture.h>
#include <f3d/engine/core/log.h>

#include <GL/glew.h>
#include <GL/gl.h>

void cubemap_init(cubemap_t *cubemap, texture_t **textures) {
    cubemap->map = texture_new();
    cubemap->textures = textures;
    texture_init(cubemap->map);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->map->id);
    int i;
    texture_t *texture;
    for (i = 0; i < 6; i++) {
        texture = cubemap->textures[i];
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
            0, GL_RGB, texture->image.width, texture->image.height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, texture->image.data
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
