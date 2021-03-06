#include <f3d/engine/object/object.h>
#include <f3d/engine/object/material.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/util.h>
#include <f3d/engine/limits.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


render_object_t render_objects[MAX_RENDER_OBJECTS];
int render_objects_index = 0;

void objects_sort(void);

render_object_t *object_new(const char *name) {
    int index = render_objects_index++;
    if (index > MAX_RENDER_OBJECTS) {
        log_msg(LOG_WARN, "Render objects index > render objects size (%d > %d)\n", render_objects_index, MAX_RENDER_OBJECTS);
        render_objects_index = 0;
        index = 0;
    }
    render_object_t *object = &render_objects[index];
    strcpy(object->name, name);
    object->hash = util_hash_str(name);
    object->mesh = NULL;
    object->position = (vector3f_t){0, 0, 0};
    object->rotation = (vector3f_t){0, 0, 0};
    object->scale = (vector3f_t){1, 1, 1};
    object->material = NULL;
    
    object_update(object);
    
    return object;
}

void render_set_target(int target, void *ptr) {
    if (target == RENDER_TARGET_FRAMEBUFFER) {
        framebuffer_bind((framebuffer_t *)ptr);
    }
    else {
        log_msg(LOG_WARN, "Target is not a valid type\n", 0);
    }
}

render_object_t *render_object_get(const char *name) {
    hash_t hash = util_hash_str(name);
    int i;
    for (i = 0; i < render_objects_index; i++) {
        if (render_objects[i].hash == hash)
            return &render_objects[i];
    }
    return NULL;
}

void scale_object(render_object_t *object) {
    //int i;
    mat4_t *mat = &object->matrix;
    // x
    mat->val[0] *= object->scale.x;
    mat->val[1] *= object->scale.x;
    mat->val[2] *= object->scale.x;
    mat->val[3] *= object->scale.x;
    
    mat->val[4] *= object->scale.y;
    mat->val[5] *= object->scale.y;
    mat->val[6] *= object->scale.y;
    mat->val[7] *= object->scale.y;
    
    mat->val[8] *= object->scale.z;
    mat->val[9] *= object->scale.z;
    mat->val[10] *= object->scale.z;
    mat->val[11] *= object->scale.z;
}

void object_move_v(render_object_t *object, vector3f_t val) {
    object->position = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}

void object_rotate_v(render_object_t *object, vector3f_t val) {
    object->rotation = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}
void object_scale_v(render_object_t *object, vector3f_t val) {
    object->scale = val;
    object->flags |= RENDER_OBJECT_FLAG_UPDATE;
}

void object_move(render_object_t *object, float x, float y, float z) {
    object_move_v(object, (vector3f_t){x, y, z});
}

void object_rotate(render_object_t *object, float x, float y, float z) {
    object_rotate_v(object, (vector3f_t){x, y, z});
}

void object_scale(render_object_t *object, float x, float y, float z) {
    object_scale_v(object, (vector3f_t){x, y, z});
}

void object_update(render_object_t *object) {
    // translations
    mat4_translate(&object->matrix, object->position);
    
    // do rotations
    object->matrix = mat4_rotate_x(object->matrix, object->rotation.x);
    object->matrix = mat4_rotate_y(object->matrix, object->rotation.y);
    object->matrix = mat4_rotate_z(object->matrix, object->rotation.z);
    
    scale_object(object);
}

void object_attach(render_object_t *object, int type, void *data) {
    if (type == OBJECT_ATTACH_MESH) {
        object->mesh = (mesh_t *)data;
    }
    else if (type == OBJECT_ATTACH_MATERIAL) {
        object->material = (material_t *)data;
    }
    else {
        log_msg(LOG_ERROR, "Attach type is not valid\n", 0);
    }
}
hash_t get_material_hash(render_object_t *object) {
    if (object->material == NULL)
        return 0;
    return object->material->hash;
}

int compare_materials(const void *v1, const void *v2) {
    render_object_t *obj1 = (render_object_t *)v1;
    render_object_t *obj2 = (render_object_t *)v2;

    return get_material_hash(obj1)-get_material_hash(obj2);
}

void objects_sort(void) {
    qsort(render_objects, render_objects_index, sizeof(render_object_t), &compare_materials);
}

void objects_draw(shader_t *shader, camera_t *camera, bool render_materials) {
    int i;
    hash_t mat_hash = 1;
    render_object_t *object;
    for (i = 0; i < render_objects_index; i++) {
        object = &render_objects[i];
        if (render_materials == false) {
            material_update(NULL, shader);
        }
        else {
            if (mat_hash != get_material_hash(object)) {
                material_update(object->material, shader);
                mat_hash = get_material_hash(object);
            }        
        }
        if (object->flags & RENDER_OBJECT_FLAG_UPDATE) {
            object_update(object);
        }
        if (object->mesh != NULL) {
            mesh_draw(object->mesh, &object->matrix, camera, shader);
        }
    }
}
