#include "app.h"
#include "gameobjects.h"

#include <SDL2/SDL_image.h>

void init_app(App *app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0)
    {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Cube!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL)
    {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0)
    {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL)
    {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    // SDL_ShowCursor(SDL_DISABLE);
    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;

    app->last_position = (vec3){3.67, 3.471, 1.0};
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO)
    {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 50);
}

void handle_app_events(App *app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;
    int speed = 2;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                if (!(app->is_map_open))
                    set_camera_speed(&(app->camera), speed);
                break;
            case SDL_SCANCODE_S:
                if (!(app->is_map_open))
                    set_camera_speed(&(app->camera), -speed);
                break;
            case SDL_SCANCODE_A:
                if (!(app->is_map_open))
                    set_camera_side_speed(&(app->camera), speed);
                break;
            case SDL_SCANCODE_D:
                if (!(app->is_map_open))
                    set_camera_side_speed(&(app->camera), -speed);
                break;
            case SDL_SCANCODE_1:
                if (app->scene.light_intensity <= 50.0f)
                    app->scene.light_intensity += 0.1f;
                break;
            case SDL_SCANCODE_2:
                if (app->scene.light_intensity >= 0.0f)
                    app->scene.light_intensity -= 0.1f;
                break;
            case SDL_SCANCODE_E:
                open_door(app);
                break;
            case SDL_SCANCODE_F1:
                open_help(app);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_M:
                open_map(app, &(app->camera));
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down && !(app->is_map_open))
            {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App *app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene));

    app->scene.light_position = (vec3){app->camera.position.x, app->camera.position.y, 1.0f};

    app->scene.gameobjects.key_position = (vec3){app->camera.newPosition.x, app->camera.newPosition.y, 1.0f};
    app->scene.gameobjects.key_rotation = (vec3){app->camera.rotation.x, app->camera.rotation.y, app->camera.rotation.z};

    app->scene.gameobjects.marker_z = (sin(current_time * 2) / 8) + 2.9;

    if(!app->scene.is_key_picked_up)
        app->scene.gameobjects.key_marker_x = (sin(current_time * 2) / 8) + 2.9;
    else
        app->scene.gameobjects.key_marker_x = -3.0f;

    app->scene.gameobjects.key_rotation_z = (current_time) * 120;

    if (app->scene.is_door_open && app->scene.gameobjects.gate_position.z > 0.0f)
    {
        app->scene.gameobjects.gate_position.z -= 0.01f;
    }
    else if (app->scene.is_door_open)
    {
        app->scene.is_door_open = false;
    }

    //printf("x: %f , y: %f \n", app->camera.position.x, app->camera.position.y);
    pickup_key(app, &(app->camera));


    app->camera.is_door_open = app->scene.is_door_open;

    app->scene.gameobjects.is_game_over = app->camera.is_game_over;


}

void render_app(App *app)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible)
    {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}
void destroy_app(App *app)
{
    if (app->gl_context != NULL)
    {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

void open_map(App *app, Camera *camera)
{
    if (app->is_map_open == 1)
    {
        glEnable(GL_FOG);
        //app->scene.light_intensity = 0.0f;
        app->scene.is_map_open = 0;
        camera->position = app->last_position;
        camera->rotation = app->last_rotation;
        app->is_map_open = 0;
    }
    else
    {
        glDisable(GL_FOG);
        app->scene.light_intensity = 1.0f;
        app->last_position = (vec3){camera->position.x, camera->position.y, camera->position.z};
        app->last_rotation = (vec3){camera->rotation.x, camera->rotation.y, camera->rotation.z};
        app->scene.last_position = (vec3){camera->position.x, camera->position.y, camera->position.z};
        app->scene.gameobjects.last_position = (vec3){camera->position.x, camera->position.y, camera->position.z};


        app->is_map_open = 1;
        app->scene.is_map_open = 1;

        camera->position = (vec3){-10.0f, 0.0f, 7.0f};
        camera->rotation = (vec3){-30.0f, 0.0f, 0.0f};
    }
}

void open_door(App *app)
{
    if (app->scene.is_key_picked_up)
    {
        app->scene.is_door_open = true;
    }
}

void pickup_key(App *app, Camera *camera)
{
    if(abs(app->scene.gameobjects.pickup_key_position.x - camera->position.x) < 0.2f && abs(app->scene.gameobjects.pickup_key_position.y - camera->position.y) < 0.2f){
        app->scene.is_key_picked_up = true;
        app->scene.gameobjects.pickup_key_position = (vec3){-3.0f, -3.0f, -3.0f}; 
    }
}

void open_help(App *app){
    if (app->scene.gameobjects.is_help_open)
    {
        app->scene.gameobjects.is_help_open = false;
    }
    else
    {
        app->scene.gameobjects.is_help_open = true;
    }
}