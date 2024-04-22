#include "raylib.h"
#include <cmath>
#include <iostream>


int main() {

    //===============================================================
    //Initializations

    //Change these two values based on your current screen dpi if you want fullscreen
    //GetMonitorWidth(0)/ GetMonitorHeight(0) seem to not work correctly
    int screenWidth = 3000; 
    int screenHeight = 2000;
    InitWindow(screenWidth, screenHeight, "Particle Life");

    int textSize = 40; 

    const char* msg("WELCOME TO PARTICLE LIFE SIM");
    int msgWidth = MeasureText(msg, textSize);
    int msgX = (screenWidth - msgWidth) / 2;
    int msgY = screenHeight / 2 - textSize / 2;

    SetTargetFPS(60);

    //===============================================================
    //Main loop

    while (!WindowShouldClose()) {
        //updates

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText(msg, msgX, msgY, textSize, BLACK);


        EndDrawing();

    }

    //===============================================================
    //End

    CloseWindow();
    return 0;
}
