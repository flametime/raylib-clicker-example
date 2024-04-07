#include "raylib.h"
#include "rlgl.h"
#include "data.c"
#include "include/reasings.h" 

const int screenWidth = 800;
const int screenHeight = 450;
Camera2D camera;
Vector2 rect_position;
Vector2 rect_size = {100,100};
Vector2 rect_default = {100,100};
int clicks = 0;
int frames = 10;
struct text_tween text[60];

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(400, 800, "raylib [core] example - basic window");

    SetTargetFPS(60);

    player_setup();
    
    while (!WindowShouldClose())
    {
        frames += 1;

        BeginDrawing();
            BeginMode2D(camera);

            ClearBackground(RAYWHITE);
            
            //Rectangle base position 
            rect_position = (Vector2){GetScreenWidth()/2 - rect_size.x/2, GetScreenHeight()/2 - rect_size.y/2};

            DrawRectangle(rect_position.x, rect_position.y, rect_size.x, rect_size.y, RED);

            //Draw text with click count
            char* str[20];
            sprintf(str, "%d", clicks);
            DrawText(str, rect_position.x + rect_size.x/2, rect_position.y + rect_size.y + 50, clicks, BLACK);
            

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 position = GetMousePosition();

                //Check if click inside rectangle
                if(position.y > rect_position.y & position.y < rect_position.y + rect_size.y)
                {
                    //Check if click inside rectangle
                    if(position.x > rect_position.x & position.x < rect_position.x + rect_size.x)
                    {
                        clicks += 1;

                        //Set frames to 0 for tween start(rectangle weight and height), look tween() function lower in this code
                        frames = 0;

                        //Get lenght of array with pop-ups (Click!) 
                        int array_length = sizeof(text) / sizeof(text[0]);

                        //Find finished pop (if pop up frame count == 60 its mean pop up tween is finished, so we can reuse it again)
                        for (int i = 0; i < array_length; i++)
                        {
                            if(text[i].frame_count == 60)
                            {   
                                //Reset pop-up frame count
                                text[i].frame_count = 0;
                                //Set initial pop-up position
                                Vector2 mouse = GetMousePosition();
                                mouse.x -= 30;
                                text[i].position = mouse;
                                break;
                            }
                        }
                        
                    }
                   
                }
            }

            //This code block tick all pop-ups every frame
            int array_length = sizeof(text) / sizeof(text[0]);  

            for (int i = 0; i < array_length; i++)
            {
                //If pop-up frame count < 60 its mean pop up not finish tween, so we update this pop-up
                if(text[i].frame_count < 60)
                {   
                    //Pop-up tween
                    tween_te(&text[i]);
                    text[i].frame_count++;
                }
            }
            //Rectangle tween, update every frame
            tween(&rect_size);
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void tween_te(struct text_tween* pos)
{
    if (frames >= 60)
        return;
    //Tween text with ease 
    pos->position.y = EaseBackOut(pos->frame_count, pos->position.y, -5, 60);
    
    DrawText("Click!", pos->position.x, pos->position.y, 25, BLACK);
}

//Tween Rectangle
void tween(Vector2* pos)
{
    if (frames >= 10)
        return;

    if (frames == 9)
    {
        pos->x = EaseBackOut(frames, pos->x, rect_default.x - pos->x, 10);
        pos->y = EaseBackOut(frames, pos->y, rect_default.y - pos->y, 10);
    }

    pos->x = EaseBackOut(frames, pos->x, GetRandomValue(-5, 5), 10);
    pos->y = EaseBackOut(frames, pos->y, GetRandomValue(-5, 5), 10);
}

//Some base stuff
void player_setup()
{
    camera = (Camera2D){ 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}
