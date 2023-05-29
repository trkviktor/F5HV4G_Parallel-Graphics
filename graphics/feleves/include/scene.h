#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "utils.h"

#include <obj/model.h>

#include <stdbool.h>

typedef struct Scene
{
    Model maze;
    GLuint maze_texture_id;

    Model marker;
    GLuint marker_texture_id;
    float marker_z;

    Model key;
    vec3 key_position;
    vec3 key_rotation;
    GLuint key_texture_id;

    Model gate;
    vec3 gate_position;

    GLuint battery_texture_id;

    vec3 light_position;
    float light_intensity;

    bool is_map_open;
    vec3 last_position;

} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material *material);

/**
 * Update the scene.
 */
void update_scene(Scene *scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene *scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

#endif /* SCENE_H */
