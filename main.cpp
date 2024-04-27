#include "raylib.h"
#include "particle.h"
#include <vector>
#include <random>
#include <cmath>

//=========================================================================================
//3D PARTICLE LIFE SIMULATOR (Using the Raylib Library, https://www.raylib.com/ ) 
//=========================================================================================

//Function Declarations
void initParticles(int maxP, std::vector<particle>& pVector, float (&attrM)[6][6]);
void drawParticles(std::vector<particle>& pVector, const int& attrR, const float (&attrM)[6][6]);
void changeAttraction(float (&attrM)[6][6]);
float getAttractionVal();
void pWithinRadius(const int& radius, std::vector<particle>& pVector, const float (&attrM)[6][6]);
bool distanceCalc(const int& radius, const particle& p1, const particle& p2);
void drawBorder();

//=========================================================================================
//MAIN FUNCITON
int main() {
    //Initializations
    //Change these two values based on your current screen dpi if you want fullscreen.
    //GetMonitorWidth() and GetMonitorHeight() seem to cause problems for me (due to my dpi).
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
    int attractionRadius = 25;
    int maxParticles = 100;
    std::vector<particle> particles;
    float attractionMat[6][6];
    initParticles(maxParticles, particles, attractionMat);
    
    //Main loop
    while (!WindowShouldClose()) {
        //Updates
        UpdateCamera(&camera, 4);
        if(IsKeyDown(KEY_SPACE)){
            changeAttraction(attractionMat);
        }

        BeginDrawing();
           
            ClearBackground(BLACK);
            BeginMode3D(camera); 

                drawBorder();
                drawParticles(particles, attractionRadius, attractionMat);

            EndMode3D();  
            DrawFPS(20, 20);
            DrawText(" - SPACE to change attraction values", 100, 20, 20, GREEN);

        EndDrawing();

    }

    CloseWindow();
    return 0;
}

//=========================================================================================
//Creates all particles with random col, and starting position/velocity.
void initParticles(int maxP, std::vector<particle>& pVector, float (&attrM)[6][6]){
    for(int i = 0; i < maxP; i++){
        float posX = GetRandomValue(-50, 50);
        float posY = GetRandomValue(0, 30);
        float posZ = GetRandomValue(-50, 50);
        int colNum = GetRandomValue(1, 6);

        Vector3 velocity = {
            (float)GetRandomValue(-5, 5),
            (float)GetRandomValue(-5, 5),
            (float)GetRandomValue(-5, 5)
        };

        particle p((Vector3){posX, posY, posZ}, velocity, colNum);
        pVector.push_back(p);
        changeAttraction(attrM);
    }
}

//=========================================================================================
//Renders all the prarticles, and updates them for next draw.
void drawParticles(std::vector<particle>& pVector, const int& attrR, const float (&attrM)[6][6]){
    for(auto& p : pVector){
        p.drawParticle();
        pWithinRadius(attrR, pVector, attrM);
        p.updateParticle(GetFrameTime());
    }
}

//=========================================================================================
//Changes the values stores in the attraction 2d array (matrix).
void changeAttraction(float (&attrM)[6][6]){
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 6; j++){
            attrM[i][j] = getAttractionVal();
        }
    }
}

//=========================================================================================
//Gives a random float value between 0.0 and 1.0
float getAttractionVal(){
    static std::mt19937 random(std::random_device{}());
    static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    return distribution(random);
}

//=========================================================================================
//Checks for particles within radius of each particle and updates values.
//NOTE FOR ME: THIS MAY NEED TO CHANGE DUE TO BOTTLENECK, GRID HASH?
void pWithinRadius(const int& radius, std::vector<particle>& pVector, const float (&attrM)[6][6]){
    for(auto& p : pVector){
       for(auto& otherP : pVector){
            if (p == otherP){
                continue;
            }
            if(distanceCalc(radius, p, otherP)){
                p.colorInteraction(otherP, attrM);
            }
        }
    }
}

//=========================================================================================
//Pos subtraction to check distance using radius.
bool distanceCalc(const int& radius, const particle& p1, const particle& p2){
    return ((abs(p1.pos.x - p2.pos.x) <= radius && abs(p1.pos.y - p2.pos.y) <= radius 
            && abs(p1.pos.z - p2.pos.z) <= radius));
}

//=========================================================================================
//Draws a box that contains the particles.
void drawBorder(){
    DrawLine3D((Vector3){-50.0f, 0.0f, -50.0f}, (Vector3){-50.0f, 0.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 0.0f, -50.0f}, (Vector3){50.0f, 0.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 0.0f, 50.0f}, (Vector3){50.0f, 0.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 0.0f, 50.0f}, (Vector3){50.0f, 0.0f, -50.0f}, LIGHTGRAY);
    
    DrawLine3D((Vector3){-50.0f, 30.0f, -50.0f}, (Vector3){-50.0f, 30.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 30.0f, -50.0f}, (Vector3){50.0f, 30.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 30.0f, 50.0f}, (Vector3){50.0f, 30.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 30.0f, 50.0f}, (Vector3){50.0f, 30.0f, -50.0f}, LIGHTGRAY);
    
    DrawLine3D((Vector3){-50.0f, 0.0f, -50.0f}, (Vector3){-50.0f, 30.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 0.0f, -50.0f}, (Vector3){50.0f, 30.0f, -50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){-50.0f, 0.0f, 50.0f}, (Vector3){-50.0f, 30.0f, 50.0f}, LIGHTGRAY);
    DrawLine3D((Vector3){50.0f, 0.0f, 50.0f}, (Vector3){50.0f, 30.0f, 50.0f}, LIGHTGRAY);
}