#include "raylib.h"

//===================================================================
//PARTICLE LIFE SIM 3D (Using Raylib) 
//===================================================================


int main() {

    //===============================================================
    //Initializations

    //Change these two values based on your current screen dpi if you want fullscreen
    //GetMonitorWidth(0)/ GetMonitorHeight(0) seem to not work correctly
    int screenWidth = 3000; 
    int screenHeight = 2000;
    InitWindow(screenWidth, screenHeight, "Particle Life");
    SetTargetFPS(60);

    Camera3D camera = {0};
    camera.fovy = 90.0f;
    camera.position = (Vector3){10.0f, 10.0f, 10.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;


    //===============================================================
    //Main loop

    while (!WindowShouldClose()) {
        //Updates
        UpdateCamera(&camera, 3);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera); 
                DrawGrid(10, 10.0f);
            EndMode3D();  

        EndDrawing();

    }

    //===============================================================
    //End of Loop

    CloseWindow();
    return 0;
}
