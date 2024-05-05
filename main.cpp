#include "raylib.h"
#include "raymath.h"
#include "particle.h"
#include <vector>
#include <map>
#include <random>
#include <cmath>
#include <string>
#include <tuple>
#include <algorithm>
#include <iostream>

//=========================================================================================
//3D PARTICLE LIFE SIMULATOR (Using the Raylib Library, https://www.raylib.com/ ) 
//=========================================================================================

//Function Declarations
void initParticles(int maxP, std::vector<particle>& pVector, float (&attrM)[6][6], std::map <std::tuple <int, int, int>, std::vector<particle>>& map, int xz, int y);
void changeAttraction(float (&attrM)[6][6]);
float getAttractionVal();
void pWithinRadius(const int& radius, std::vector<particle>& pVector, const float (&attrM)[6][6], std::map <std::tuple <int, int, int>, std::vector<particle>>& map);
bool distanceCalc(const int& radius, const particle& p1, const particle& p2);
void updateSections(std::map <std::tuple <int, int, int>, std::vector<particle>>& map, std::vector<particle>& pVector, int mapSectionSize, int ySectionSize);
std::tuple <int, int, int> posHashKey(const particle& p, int mapSectionSize, int ySectionSize);
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
    int attractionRadius = 25;//25
    int maxParticles = 1600;
    std::vector<particle> particles;
    float attractionMat[6][6];
    std::map <std::tuple <int, int, int>, std::vector<particle>> sectionMap; 
    int sectionSize = 25;
    int ySectionSize = 15;
    initParticles(maxParticles, particles, attractionMat, sectionMap, sectionSize, ySectionSize);
    
    //Main loop
    while (!WindowShouldClose()){
        
        UpdateCamera(&camera, 4);
        if(IsKeyDown(KEY_SPACE)){
            changeAttraction(attractionMat);
        }

        //Render loop
        BeginDrawing();
           
            ClearBackground(BLACK);
            BeginMode3D(camera); 
                drawBorder();
                for(auto& particle : particles){
                    particle.updateParticle(GetFrameTime());
                    particle.drawParticle(); 
                }
                updateSections(sectionMap, particles, sectionSize, ySectionSize);
                pWithinRadius(attractionRadius, particles, attractionMat, sectionMap); 
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
void initParticles(int maxP, std::vector<particle>& pVector, float (&attrM)[6][6], std::map <std::tuple <int, int, int>, std::vector<particle>>& map, int xz, int y){
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
        p.id = i + 1;
        p.currentHash = posHashKey(p, xz, y);
        pVector.push_back(p);
        changeAttraction(attrM);
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
//Gives a random float value between 0.0 and 1.0.
float getAttractionVal(){
    static std::mt19937 random(std::random_device{}());
    static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    return distribution(random);
}

//=========================================================================================
//Checks for particles in the same and adjasent sections (to avoid checking all particles).
void pWithinRadius(const int& radius, std::vector<particle>& pVector, const float (&attrM)[6][6], std::map <std::tuple <int, int, int>, std::vector<particle>>& map){
    std::vector<std::tuple <int,int,int>> hashKeys;
    for(auto& p : pVector){
        Vector3 totalForce = Vector3Zero();
        hashKeys.clear();
        int x, y, z;
        std::tie(x, y, z) = p.currentHash;
        hashKeys.push_back(p.currentHash);
        hashKeys.push_back(std::make_tuple(x + 1, y, z));
        hashKeys.push_back(std::make_tuple(x - 1, y, z));
        hashKeys.push_back(std::make_tuple(x, y + 1, z));
        hashKeys.push_back(std::make_tuple(x, y - 1, z));
        hashKeys.push_back(std::make_tuple(x, y, z + 1));
        hashKeys.push_back(std::make_tuple(x, y, z - 1));

        for(auto& key : hashKeys){
            auto it = map.find(key);
            if(it != map.end()) {
                for(auto& otherP : it->second){
                if(p.id == otherP.id) continue; 
                if(distanceCalc(radius, p, otherP)){
                    Vector3 otherForce = p.colorInteraction(otherP, attrM); 
                    totalForce = Vector3Add(totalForce, otherForce);
                  }
                }
            }
        }  
        p.force = totalForce;
    }
}

//=========================================================================================
//Pos subtraction to check distance using radius.
bool distanceCalc(const int& radius, const particle& p1, const particle& p2){
    return ((abs(p1.pos.x - p2.pos.x) <= radius && abs(p1.pos.y - p2.pos.y) <= radius 
            && abs(p1.pos.z - p2.pos.z) <= radius));
}

//=========================================================================================
//Adds particles to their new map sections (goal to avoid bottlenecks).
void updateSections(std::map <std::tuple <int, int, int>, std::vector<particle>>& map, std::vector<particle>& pVector, int mapSectionSize, int ySectionSize){
    for(auto& p : pVector){
        std::tuple sectionHash = posHashKey(p, mapSectionSize, ySectionSize);
        if(sectionHash == p.currentHash){
            continue;
        }
        else{ 
            auto it = map.find(p.currentHash);
            if(it != map.end()){
                auto& vec = it->second;
                auto vecIt = std::find_if(vec.begin(), vec.end(), [&](const particle& item){return item.id == p.id;});
                if(vecIt != vec.end()){
                    vec.erase(vecIt);
                }
            }
            map[sectionHash].push_back(p); 
            p.currentHash = sectionHash;
        }
    }
}

//=========================================================================================
//Creates string hash key based on particle position.
std::tuple <int, int, int> posHashKey(const particle& p, int mapSectionSize, int ySectionSize){
    int x, y, z;
    
    x = ((int)floor(p.pos.x / mapSectionSize));
    y = ((int)floor(p.pos.y / ySectionSize));
    z = ((int)floor(p.pos.z / mapSectionSize));
    
    if(x == 2){x = 1;}
    if(y == 2){y = 1;}
    if(z == 2){z = 1;}
    
    return std::make_tuple(x, y, z);
}

//=========================================================================================
//Draws the box that contains the particles.
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