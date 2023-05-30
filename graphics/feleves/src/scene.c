#include "scene.h"
#include <obj/info.h>
#include <obj/load.h>
#include <obj/draw.h>
#include "gameobjects.h"

void init_scene(Scene *scene)
{
    load_model(&(scene->maze), "assets/models/maze.obj");
    

    init_gameobjects(&(scene->gameobjects));

    scene->maze_texture_id = load_texture("assets/textures/maze.png");
    scene->light_position = (vec3){0.0, 0.0, 0.0};
    scene->light_intensity = 0.2f;
    scene->is_map_open = 0;
    scene->is_key_picked_up = false;

    float fog[4] = {0.5f, 0.5f, 0.5f, 0.5f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_FOG);
    glFogf(GL_FOG_DENSITY, 0.0005f);
    glFogfv(GL_FOG_COLOR, fog);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 3.5);

    scene->is_door_open = false;
}
void set_lighting(Scene *scene)
{
    float ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float diffuse_light[] = {scene->light_intensity, scene->light_intensity, scene->light_intensity, 1.0f};
    float specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float position[] = {scene->light_position.x, scene->light_position.y, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material *material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue};

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue};

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene *scene)
{
    // printf("last position: x:%f y:%f z:%f\n", scene->last_position.x, scene->last_position.y, scene->last_position.z);
}

void render_scene(const Scene *scene)
{
    set_lighting(scene);

    render_maze(scene);
    render_gameobjects(&(scene->gameobjects));

    
    if (scene->is_key_picked_up){
        render_key_in_hand(&(scene->gameobjects));
    }

    if (scene->is_map_open)
        render_player_marker(&(scene->gameobjects));
        
        
    set_lighting(scene);
}

void render_maze(const Scene *scene){
    set_lighting(scene);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->maze_texture_id);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0.5, 0);
    // glScalef(2.0, 2.0, 2.0);
    // draw_origin();
    draw_model(&(scene->maze));
    glPopMatrix();
}


