#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "camera.h"
#include "texture.h"
#include "utils.h"

#include <obj/model.h>

#include <stdbool.h>

typedef struct Gameobjects
{
    
    Model marker;
    GLuint marker_texture_id;
    float marker_z;

    Model key;
    vec3 key_position;
    vec3 key_rotation;
    vec3 pickup_key_position;

    GLuint key_texture_id;

    float key_rotation_z;
    float key_marker_x;

    Model gate;
    vec3 gate_position;

    vec3 last_position;

    bool is_help_open;
    bool is_game_over;

} Gameobjects;


void init_gameobjects(Gameobjects *gameobjects);

void render_gameobjects(const Gameobjects *gameobjects);

void render_marker(const Gameobjects *gameobjects);

void render_key_in_hand(const Gameobjects *gameobjects);

void render_key_pickup(const Gameobjects *gameobjects);

void render_gate(const Gameobjects *gameobjects, vec3 position);

void render_player_marker(const Gameobjects *gameobjects);

void render_help();

void render_game_over();



#endif /* GAMEOBJECTS_H */
