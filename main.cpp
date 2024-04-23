#include "raylib.h"
#include "particle.h"
#include <vector>

//===================================================================
//3D PARTICLE LIFE SIMULATOR (Using the Raylib Library) 
//===================================================================

void initParticles(int maxP, std::vector<particle>& pVector);
void drawParticles(std::vector<particle> pVector);
void drawBorder();

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

    //Camera settings
    Camera3D camera;
    camera.fovy = 70.0f;
    camera.position = (Vector3){30.0f, 30.0f, 30.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    //Particle settings
    std::vector<particle> particles;
    int maxParticles = 700;
    initParticles(maxParticles, particles);

    //===============================================================
    //Main loop

    while (!WindowShouldClose()) {
        //Updates
        UpdateCamera(&camera, 4);

        BeginDrawing();
           
            ClearBackground(BLACK);
            BeginMode3D(camera); 

                drawBorder();
                drawParticles(particles);

            EndMode3D();  
            DrawFPS(20, 20);

        EndDrawing();

    }

    //===============================================================
    //End of Loop

    CloseWindow();
    return 0;
}

//Particle array functions
void initParticles(int maxP, std::vector<particle>& pVector){
    for(int i = 0; i < maxP; i++){
        float posX = GetRandomValue(-50, 50);
        float posY = GetRandomValue(0, 20);
        float posZ = GetRandomValue(-50, 50);
        int colNum = GetRandomValue(1, 6);
        Color col;
        switch(colNum) {
            case 1:
                col = PINK;
                break;
            case 2:
                col = SKYBLUE;
                break;
            case 3:
                col = LIME;
                break;
            case 4:
                col = RED;
                break;
            case 5:
                col = WHITE;
                break;
            case 6:
                col = YELLOW;
                break;
            default:
                col = BLACK;
        }

        particle p((Vector3){posX, posY, posZ}, (Vector3){0.0f, 0.0f, 0.0f}, col);
        pVector.push_back(p);
    }
}

void drawParticles(std::vector<particle> pVector){
    for(auto& p : pVector){
        p.drawParticle();
    }
}

void drawBorder(){
    DrawLine3D((Vector3){-50.0f, 0.0f, -50.0f}, (Vector3){-50.0f, 0.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 0.0f, -50.0f}, (Vector3){50.0f, 0.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 0.0f, 50.0f}, (Vector3){50.0f, 0.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 0.0f, 50.0f}, (Vector3){50.0f, 0.0f, -50.0f}, LIGHTGRAY);
    
    DrawLine3D((Vector3){-50.0f, 20.0f, -50.0f}, (Vector3){-50.0f, 20.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 20.0f, -50.0f}, (Vector3){50.0f, 20.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 20.0f, 50.0f}, (Vector3){50.0f, 20.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 20.0f, 50.0f}, (Vector3){50.0f, 20.0f, -50.0f}, LIGHTGRAY);
    
    DrawLine3D((Vector3){-50.0f, 0.0f, -50.0f}, (Vector3){-50.0f, 20.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 0.0f, -50.0f}, (Vector3){50.0f, 20.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 0.0f, 50.0f}, (Vector3){-50.0f, 20.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 0.0f, 50.0f}, (Vector3){50.0f, 20.0f, 50.0f}, LIGHTGRAY);
}