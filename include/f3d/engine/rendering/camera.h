#ifndef F3D_CAMERA_H
#define F3D_CAMERA_H

#include <f3d/engine/types.h>

#define CAMERA_FORWARD   0
#define CAMERA_BACKWARD  1
#define CAMERA_LEFT      2
#define CAMERA_RIGHT     3


typedef struct {
    vector3f_t position;
    vector3f_t rotation;
    
    vector3f_t right, direction, up;
    
    float move_speed;
    vector3f_t move_mul;
    float fov;
    
    mat4_t mat_view, mat_projection;
} camera_t;
extern camera_t *selected_camera;

camera_t camera_new(void);
void camera_select(camera_t *camera);
void camera_clamp_rotation(camera_t *camera);
void camera_move(camera_t *camera, int direction);
void camera_update(camera_t *camera);

#endif
