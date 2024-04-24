#include "particle.h"
#include "raymath.h"

//=========================================================================================
particle::particle(Vector3 p, Vector3 v, int c) 
: pos(p), velocity(v), color(c){}

//=========================================================================================
//Draws the particle based on its current position.
void particle::drawParticle(){
    Color col;
    switch(this->color){
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
        };
    //DrawSphereEx(this->pos, 0.08f, 3, 3, col);
    DrawCircle3D(this->pos, 0.08f, Vector3{0.0, 1.0, 0.0}, 90.0f, col);
}

//=========================================================================================
//Updates the position, velocity, etc for the current particle.
void particle::updateParticle(float time){
    float damping = 0.9; 
    this->velocity.x *= damping;
    this->velocity.y *= damping;
    this->velocity.z *= damping;

    this->pos.x += this->velocity.x * time;
    this->pos.y += this->velocity.y * time;
    this->pos.z += this->velocity.z * time;

    if(this->pos.x < -50){
        this->pos.x = 50;
    }
    if(this->pos.x > 50){
        this->pos.x = -50;
    }
    if(this->pos.z < -50){
        this->pos.z = 50;
    }
    if(this->pos.z > 50){
        this->pos.z = -50;
    }
    if(this->pos.y < 0){
        this->pos.y = 30;
    }
    if(this->pos.y > 30){
        this->pos.y = 0;
    }   
}

//=========================================================================================
//Determines how a particle should react based on (other) particle p (within radius).
void particle::colorInteraction(const particle& p, const float (&attrM)[6][6]){
    Vector3 directionVec = Vector3Subtract(this->pos, p.pos);
    Vector3 nDirectionVec = Vector3Normalize(directionVec);
    float attraction = attrM[this->color][p.color] * .01;
    Vector3 forceVec = Vector3Scale(nDirectionVec, attraction);
    this->velocity.x += forceVec.x;
    this->velocity.y += forceVec.y;
    this->velocity.z += forceVec.z;
}

//=========================================================================================
//Operator overload for ==.
bool particle::operator==(particle const& right){
    return (this->pos.x == right.pos.x && this->pos.y == right.pos.y && 
            this->pos.z == right.pos.z && this->velocity.x == right.velocity.x && 
            this->velocity.y == right.velocity.y && this->velocity.z == right.velocity.z);
}