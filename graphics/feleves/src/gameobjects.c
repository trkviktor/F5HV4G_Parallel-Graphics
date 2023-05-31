#include "gameobjects.h"

void init_gameobjects(Gameobjects *gameobjects)
{

    load_model(&(gameobjects->marker), "assets/models/marker.obj");
    load_model(&(gameobjects->key), "assets/models/key.obj");
    load_model(&(gameobjects->gate), "assets/models/gate.obj");

    gameobjects->marker_texture_id = load_texture("assets/textures/marker.png");
    gameobjects->key_texture_id = load_texture("assets/textures/maze.png");
    gameobjects->pickup_key_position = (vec3){-3.765823, -1.248852, 1.0f};

    gameobjects->marker_z = 5.0f;
    gameobjects->key_rotation_z = 90.0f;

    gameobjects->gate_position = (vec3){3.087181f, 2.485501f, 0.9f};
    gameobjects->last_position = (vec3){0.0, 0.0, 0.0};

    gameobjects->is_help_open = false;
    gameobjects->is_game_over = false;
}

void render_gameobjects(const Gameobjects *gameobjects)
{
    //render_marker(gameobjects);
    render_key_pickup(gameobjects);
    render_gate(gameobjects, gameobjects->gate_position);
    render_gate(gameobjects, (vec3){3.124926f, 1.240040f, gameobjects->gate_position.z});
}

void render_marker(const Gameobjects *gameobjects)
{
    glPushMatrix();
    float marker_ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float marker_diffuse_light[] = {50.0f, 50.0f, 50.0f, 1.0f};
    float marker_specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float marker_position[] = {-3.1f, -1.9f, (gameobjects->marker_z) - 2, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, marker_ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, marker_diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, marker_specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, marker_position);

    glBindTexture(GL_TEXTURE_2D, gameobjects->marker_texture_id);
    glTranslatef(-3.1, -1.9, gameobjects->marker_z);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(90, 1, 0, 0);
    draw_model(&(gameobjects->marker));
    glPopMatrix();
}

void render_key_in_hand(const Gameobjects *gameobjects)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, gameobjects->key_texture_id);
    glTranslatef(gameobjects->key_position.x, gameobjects->key_position.y, gameobjects->key_position.z);
    glRotatef(gameobjects->key_rotation.z, 0, 0, 1);
    glRotatef(-gameobjects->key_rotation.x, 0, 1, 0);
    glTranslatef(0.4f, -0.15f, -0.1f);
    glRotatef(90, 0, 1, 0);
    glRotatef(-100, 1, 0, 0);
    // glScalef(0.1, 0.1, 0.1);
    draw_model(&(gameobjects->key));
    glPopMatrix();
}

void render_key_pickup(const Gameobjects *gameobjects)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, gameobjects->key_texture_id);
    glTranslatef(-3.765823, -1.248852, gameobjects->key_marker_x - 2);
    glRotatef(90, 0, 1, 0);
    glRotatef(gameobjects->key_rotation_z, 1, 0, 0);
    draw_model(&(gameobjects->key));
    glPopMatrix();
}

void render_gate(const Gameobjects *gameobjects, vec3 position)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, gameobjects->key_texture_id);
    glTranslatef(position.x, position.y, position.z);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 0, 0, 1);
    glScalef(0.1, 0.12, 0.3);
    draw_model(&(gameobjects->gate));
    glPopMatrix();
}

void render_player_marker(const Gameobjects *gameobjects)
{
    glDisable(GL_FOG);
    glPushMatrix();
    float map_marker_ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float map_marker_diffuse_light[] = {50.0f, 50.0f, 50.0f, 1.0f};
    float map_marker_specular_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float map_marker_position[] = {gameobjects->last_position.x, gameobjects->last_position.y, gameobjects->marker_z - 2, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, map_marker_ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, map_marker_diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, map_marker_specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, map_marker_position);
    glBindTexture(GL_TEXTURE_2D, gameobjects->marker_texture_id);
    glTranslatef(gameobjects->last_position.x, gameobjects->last_position.y, gameobjects->marker_z - 1);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(90, 1, 0, 0);
    draw_model(&(gameobjects->marker));
    glPopMatrix();
}

void render_help()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, load_texture("assets/textures/help.jpg"));

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3d(-2, 1.5, -3);
    glTexCoord2f(1, 0);
    glVertex3d(2, 1.5, -3);
    glTexCoord2f(1, 1);
    glVertex3d(2, -1.5, -3);
    glTexCoord2f(0, 1);
    glVertex3d(-2, -1.5, -3);
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void render_game_over()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, load_texture("assets/textures/gameover.jpg"));

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3d(-2, 1.5, -3);
    glTexCoord2f(1, 0);
    glVertex3d(2, 1.5, -3);
    glTexCoord2f(1, 1);
    glVertex3d(2, -1.5, -3);
    glTexCoord2f(0, 1);
    glVertex3d(-2, -1.5, -3);
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
