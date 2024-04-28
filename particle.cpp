#include "particle.h"
#include "raymath.h"

//=========================================================================================
particle::particle(Vector3 p, Vector3 v, int c) 
: pos(p), velocity(v), color(c){
    int x = -10;
    int y = -10;
    int z = -10;
    this->currentHash = std::make_tuple(x, y, z);
}

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
    DrawSphereEx(this->pos, 0.08f, 3, 3, col);
    //DrawCircle3D(this->pos, 0.08f, Vector3{0.0, 1.0, 0.0}, 90.0f, col);
}

//=========================================================================================
//Updates the position, velocity, etc for the current particle.
void particle::updateParticle(float time){
    float damping = 0.95f; 
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
    Vector3 directionVec = Vector3Subtract(p.pos, this->pos);
    float distance = Vector3Length(directionVec);

    Vector3 nDirection = Vector3Normalize(directionVec);
    float scaledAttraction = attrM[this->color - 1][p.color - 1] * 0.1;
    float force;
    float minDistance = 3.0f;
    if (distance <= minDistance) {
        force = -scaledAttraction * (minDistance / distance - 1);
    } else {
        force = scaledAttraction;
    }

    Vector3 forceVec = Vector3Scale(nDirection, force);
    this->velocity = Vector3Add(this->velocity, forceVec);
}


//=========================================================================================
//Operator overload for ==.
bool particle::operator==(particle const& right){
    return (this->id == right.id);
}