#include <f3d/engine/object/material.h>
#include <f3d/engine/rendering/shader.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/util.h>

#include <string.h>

static material_t materials[MATERIALS_MAX];
static int materials_index = 0; 

material_t *material_new(material_t material) {
    material_t *mat = &materials[materials_index++];
    memcpy(mat, &material, sizeof(material_t));
    mat->hash = util_hash_str(mat->name);
    log_msg(LOG_INFO, "Creating new material '%s'\n", mat->name);
    
    handle_call(HANDLE_ON_MATERIAL_LOAD, mat);
    return mat;
}

void material_attach() {

}

void material_update(material_t *mat, shader_t *shader) {
    material_t dummy;
    // TODO: flat colours, etc.
    if (mat == NULL) {
        dummy.shininess = 32.0f;
        dummy.flags = (MATERIAL_NO_NORMALMAP | MATERIAL_NO_SPECULARMAP | MATERIAL_NO_DIFFUSE);
        mat = &dummy;
        goto setup_material;
    }
    if (mat->diffuse != NULL) {
        // assign diffuse to texture0 in fragment shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mat->diffuse->id);
    }
    else
        mat->flags |= MATERIAL_NO_DIFFUSE;
        
    // assign specular
    if (mat->specular != NULL) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mat->specular->id);
    }
    else
        mat->flags |= MATERIAL_NO_SPECULARMAP;
    
    // assign normal
    if (mat->normal != NULL) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, mat->normal->id);
    }
    else
        mat->flags |= MATERIAL_NO_NORMALMAP;
    
setup_material:;
    shader_set_int(shader, "material.use_normalmap",   !(mat->flags & MATERIAL_NO_NORMALMAP));
    shader_set_int(shader, "material.use_specularmap", !(mat->flags & MATERIAL_NO_SPECULARMAP));
    shader_set_int(shader, "material.use_diffuse",     !(mat->flags & MATERIAL_NO_DIFFUSE));
    shader_set_int(shader, "material.diffuse",  0);
    shader_set_int(shader, "material.specular", 1);
    shader_set_int(shader, "material.normal",   2);
    shader_set_float(shader, "material.shininess", mat->shininess);
}

material_t *material_get(const char *name) {
    int i;
    for (i = 0; i < materials_index; i++) {
        if (!strcmp(materials[i].name, name))
            return &(materials[i]);
    }
    // replace with 'default' material
    return NULL;
}

void material_destroy(material_t *material) {
    log_msg(LOG_INFO, "Deleting material '%s'\n", material->name);
        
    texture_destroy(material->diffuse);
    texture_destroy(material->specular);
    texture_destroy(material->normal);
    
    material->name[0] = '*';
}
