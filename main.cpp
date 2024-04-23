#include "raylib.h"

//===================================================================
//3D PARTICLE LIFE SIMULATOR (Using the Raylib Library) 
//===================================================================


int main() {
    //===============================================================
    //Initializations

    //Change these two values based on your current screen dpi if you want fullscreen.
    //GetMonitorWidth() and GetMonitorHeight() seem to cause problems for me.
    int screenWidth = 3000; 
    int screenHeight = 2000;
    InitWindow(screenWidth, screenHeight, "Particle Life");
    SetTargetFPS(60);
    DisableCursor();

    Camera3D camera;
    camera.fovy = 70.0f;
    camera.position = (Vector3){30.0f, 30.0f, 30.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;


    //===============================================================
    //Main loop

    while (!WindowShouldClose()) {
        //Updates
        UpdateCamera(&camera, 4);

        BeginDrawing();
           
            ClearBackground(BLACK);
            BeginMode3D(camera); 

                DrawGrid(10, 10.0f);
                DrawSphere((Vector3){0.0f, 10.0f, 0.0f}, 7.0f, LIME);

            EndMode3D();  
            DrawFPS(20, 20);

        EndDrawing();

    }

    //===============================================================
    //End of Loop

    CloseWindow();
    return 0;
}
