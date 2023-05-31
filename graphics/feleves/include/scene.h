#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "utils.h"
#include "gameobjects.h"

#include <windows.h>

#include <obj/model.h>

#include <stdbool.h>

typedef struct Scene
{

    Gameobjects gameobjects;

    Model maze;
    GLuint maze_texture_id;

    int is_map_open;
    vec3 last_position;

    bool is_door_open;
    bool is_key_picked_up;

    vec3 light_position;
    float light_intensity;
    




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
