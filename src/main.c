#include "../external/raylib.h"

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef signed long long i64;
typedef signed int i32;
typedef signed short i16;
typedef signed char i8;

typedef float f32;
typedef double f64;

typedef enum
{
    GAME_STATE_DEVELOPER,
    GAME_STATE_QUIT
} game_state_e;

typedef struct
{
    i32 mouse_x;
    i32 mouse_y;
    i32 mouse_prev_x;
    i32 mouse_prev_y;
    i32 mouse_delta_x;
    i32 mouse_delta_y;
    bool lmb_down;
    bool rmb_down;
    bool lmb_pressed;
    bool rmb_pressed;
    i32 key_pressed;
} input_t;

typedef struct
{
    bool fullscreen;
    i32 windowed_width;
    i32 windowed_height;
    i32 fullscreen_width;
    i32 fullscreen_height;
    i32 windowed_position_x;
    i32 windowed_position_y;
    game_state_e curr_state;
    game_state_e prev_state;
    float time_in_state;
    input_t input;
    float delta_time;
} game_state_t;

#define APP_WIDTH (game_state.fullscreen ? game_state.fullscreen_width : game_state.windowed_width)
#define APP_HEIGHT (game_state.fullscreen ? game_state.fullscreen_height : game_state.windowed_height)

game_state_t game_state = {
    .curr_state = GAME_STATE_DEVELOPER};

void (*state_enter_funcs[])(i32, i32) = {
    NULL, // developer menu
    NULL, // quit
};

i32 (*state_exit_funcs[])() = {
    NULL, // developer menu
    NULL};

i32 state_exit(game_state_e state)
{
    if ((*state_exit_funcs[state]) != NULL)
    {
        return (*state_exit_funcs[state])();
    }
    return 0;
}

void state_enter(game_state_e to, game_state_e from, i32 code)
{
    if ((*state_enter_funcs[to]) != NULL)
    {
        (*state_enter_funcs[to])(from, code);
    }
}

void state_change(game_state_e state)
{
    if (game_state.curr_state != state)
    {
        i32 state_exit_code = state_exit(game_state.curr_state);
        state_enter(state, game_state.curr_state, state_exit_code);
        game_state.prev_state = game_state.curr_state;
        game_state.curr_state = state;
    }
}

void input_update()
{
    game_state.input.mouse_prev_x = game_state.input.mouse_x;
    game_state.input.mouse_prev_y = game_state.input.mouse_y;
    game_state.input = (input_t){
        .mouse_x = ((double)GetScreenWidth() / (double)APP_WIDTH) * GetMousePosition().x,
        .mouse_y = ((double)GetScreenHeight() / (double)APP_HEIGHT) * GetMousePosition().y,
        .mouse_prev_x = game_state.input.mouse_prev_x,
        .mouse_prev_y = game_state.input.mouse_prev_y,
        .mouse_delta_x = game_state.input.mouse_x - game_state.input.mouse_prev_x,
        .mouse_delta_y = game_state.input.mouse_y - game_state.input.mouse_prev_y,
        .lmb_down = IsMouseButtonDown(MOUSE_LEFT_BUTTON),
        .rmb_down = IsMouseButtonDown(MOUSE_RIGHT_BUTTON),
        .lmb_pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON),
        .rmb_pressed = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON),
        .key_pressed = GetKeyPressed()};
}

void load_resources()
{
    // LOAD RESOURCES HERE
}

void window_init()
{
    InitWindow(0, 0, "Sphere City Prototype");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    game_state.fullscreen_width = GetScreenWidth();
    game_state.fullscreen_height = GetScreenHeight();
    game_state.windowed_width = game_state.fullscreen_width / 2;
    game_state.windowed_height = game_state.fullscreen_height / 2;
    game_state.windowed_position_x = (game_state.fullscreen_width / 2) - (game_state.windowed_width / 2);
    game_state.windowed_position_y = (game_state.fullscreen_height / 2) - (game_state.windowed_height / 2);

    SetWindowPosition(game_state.windowed_position_x, game_state.windowed_position_y);
    SetWindowSize(game_state.windowed_width, game_state.windowed_height);
}

int main()
{

    window_init();
    load_resources();

    while (!WindowShouldClose())
    {
        game_state.delta_time = GetFrameTime();

        input_update();

        if (IsKeyPressed(KEY_F11))
        {
            game_state.fullscreen = !game_state.fullscreen;
            if (game_state.fullscreen)
            {
                SetWindowSize(game_state.fullscreen_width, game_state.fullscreen_height);
                SetWindowState(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
                SetWindowPosition(0, 0);
            }
            else
            {
                SetWindowSize(game_state.windowed_width, game_state.windowed_height);
                ClearWindowState(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
                SetWindowPosition(game_state.windowed_position_x, game_state.windowed_position_y);
            }
        }

        if (IsWindowFocused())
        {
            switch (game_state.curr_state)
            {
            case GAME_STATE_DEVELOPER:
            {
            }
            break;
            case GAME_STATE_QUIT:
            {
                goto EXIT_PROCEDURE;
            }
            break;
            }
        }
        BeginDrawing();

        // RENDER /////////////////////////////////////////

        switch (game_state.curr_state)
        {
        case GAME_STATE_DEVELOPER:
        {
            ClearBackground(RAYWHITE);
        }
        break;
        }
        EndDrawing();
    }

EXIT_PROCEDURE:
    CloseWindow();

    return 0;
}