#include <f3d/engine/model/obj.h>
#include <f3d/engine/core/log.h>
#include <f3d/engine/core/handles.h>
#include <f3d/engine/object/material.h>

#include <f3d/engine/type/buffer.h>
#include <f3d/engine/type/vec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// http://paulbourke.net/dataformats/mtl/

void mtl_load(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        log_msg(LOG_ERROR, "Could not load '%s'\n", path);
        return;
    }
    // TODO: up line buffer size, use non infinite loop
    char line[128];
    material_t *mat = NULL;
    int res;
    for (;;) {
        res = fscanf(fp, "%s", line);
        if (res == EOF)
            break;
        if (!strcmp(line, "newmtl")) {
            char temp[32];
            fscanf(fp, "%s\n", temp);
            log_msg(LOG_INFO, "material:%s\n", temp);
            material_t newm;
            strcpy(newm.name, temp);
            mat = material_new(newm);
        }
        else if (!strcmp(line, "map_Kd")) {
            char temp[32];
            char image_path[64];
            strcpy(image_path, "../models/");
            fscanf(fp, "%s\n", temp);
            
            strcat(image_path, temp);
            log_msg(LOG_INFO, "load image %s\n", image_path);
            mat->diffuse = texture_load(NULL, image_path, IMAGE_BMP);
            mat->specular = mat->diffuse;
            mat->normal = NULL;
        }
        // specular exponent
        else if (!strcmp(line, "Ns")) {
            float spec_exp;
            fscanf(fp, "%f\n", &spec_exp);
            log_msg(LOG_INFO, "Spec exp: %f\n", spec_exp);
        }
        // ambient colour
        else if (!strcmp(line, "Ka")) {
            vector3f_t ambient;
            fscanf(fp, "%f %f %f\n", &ambient.x, &ambient.y, &ambient.z);
            log_msg(LOG_INFO, "ambient [%f,%f,%f]\n", ambient.x, ambient.y, ambient.z);
        }
        else if (!strcmp(line, "Kd")) {
            vector3f_t diffuse;
            fscanf(fp, "%f %f %f\n", &diffuse.x, &diffuse.y, &diffuse.z);
            log_msg(LOG_INFO, "diffuse [%f,%f,%f]\n", diffuse.x, diffuse.y, diffuse.z);
        }
        else if (!strcmp(line, "Ks")) {
            vector3f_t specular;
            fscanf(fp, "%f %f %f\n", &specular.x, &specular.y, &specular.z);
            log_msg(LOG_INFO, "specular [%f,%f,%f]\n", specular.x, specular.y, specular.z);
        }
        else if (!strcmp(line, "Ke")) {
            vector3f_t emissive;
            fscanf(fp, "%f %f %f\n", &emissive.x, &emissive.y, &emissive.z);
            log_msg(LOG_INFO, "emissive [%f,%f,%f]\n", emissive.x, emissive.y, emissive.z);
        }
        // Optical density
        else if (!strcmp(line, "Ni")) {
            vector3f_t opdensity;
            fscanf(fp, "%f %f %f\n", &opdensity.x, &opdensity.y, &opdensity.z);
            log_msg(LOG_INFO, "opdensity [%f,%f,%f]\n", opdensity.x, opdensity.y, opdensity.z);
        }
    }

    fclose(fp);
}
