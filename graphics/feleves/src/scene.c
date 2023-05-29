#include "scene.h"
#include <obj/info.h>
#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene *scene)
{
    load_model(&(scene->maze), "assets/models/maze.obj");
    load_model(&(scene->marker), "assets/models/marker.obj");
    load_model(&(scene->key), "assets/models/key.obj");
    load_model(&(scene->gate), "assets/models/gate.obj");

    scene->maze_texture_id = load_texture("assets/textures/maze.png");
    scene->marker_texture_id = load_texture("assets/textures/marker.png");
    scene->key_texture_id = load_texture("assets/textures/maze.png");

    scene->light_position = (vec3){0.0, 0.0, 0.0};
    scene->light_intensity = 0.2f;

    scene->marker_z = 5.0f;
    scene->is_map_open = false;
    scene->last_position = (vec3){0.0, 0.0, 0.0};

    scene->gate_position = (vec3){3.087181f, 2.485501f, 0.9f};

    float fog[4] = {0.5f, 0.5f, 0.5f, 0.5f};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_FOG);
    glFogf(GL_FOG_DENSITY, 0.0005f);
    glFogfv(GL_FOG_COLOR, fog);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 0);
    glFogf(GL_FOG_END, 3.5);

    //scene->is_door_open = false;
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
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->maze_texture_id);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0.5, 0);
    // glScalef(2.0, 2.0, 2.0);
    // draw_origin();
    draw_model(&(scene->maze));
    glPopMatrix();

    glPushMatrix();
    float marker_ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float marker_diffuse_light[] = {50.0f, 50.0f, 50.0f, 1.0f};
    float marker_specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float marker_position[] = {-3.1f, -1.9f, (scene->marker_z) - 2, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, marker_ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, marker_diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, marker_specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, marker_position);

    glBindTexture(GL_TEXTURE_2D, scene->marker_texture_id);
    glTranslatef(-3.1, -1.9, scene->marker_z);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(90, 1, 0, 0);
    draw_model(&(scene->marker));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->key_texture_id);
    glTranslatef(scene->key_position.x, scene->key_position.y, scene->key_position.z);
    glRotatef(scene->key_rotation.z, 0, 0, 1);
    glRotatef(-scene->key_rotation.x, 0, 1, 0);
    glTranslatef(0.4f, -0.15f, -0.1f);
    glRotatef(90, 0, 1, 0);
    glRotatef(-100, 1, 0, 0);
    // glScalef(0.1, 0.1, 0.1);
    draw_model(&(scene->key));
    glPopMatrix();

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, scene->key_texture_id);
    glTranslatef(scene->gate_position.x, scene->gate_position.y, scene->gate_position.z);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 0, 0, 1);
    glScalef(0.1, 0.12, 0.3);
    draw_model(&(scene->gate));
    glPopMatrix();

    if (scene->is_map_open)
    {
        glDisable(GL_FOG);
        glPushMatrix();
        float map_marker_ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
        float map_marker_diffuse_light[] = {50.0f, 50.0f, 50.0f, 1.0f};
        float map_marker_specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
        float map_marker_position[] = {scene->last_position.x, scene->last_position.y, scene->marker_z - 2, 1.0f};

        glLightfv(GL_LIGHT0, GL_AMBIENT, map_marker_ambient_light);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, map_marker_diffuse_light);
        glLightfv(GL_LIGHT0, GL_SPECULAR, map_marker_specular_light);
        glLightfv(GL_LIGHT0, GL_POSITION, map_marker_position);

        glBindTexture(GL_TEXTURE_2D, scene->marker_texture_id);
        glTranslatef(scene->last_position.x, scene->last_position.y, scene->marker_z - 1);
        glScalef(0.5, 0.5, 0.5);
        glRotatef(90, 1, 0, 0);
        draw_model(&(scene->marker));
        glPopMatrix();
    }
    set_lighting(scene);
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
