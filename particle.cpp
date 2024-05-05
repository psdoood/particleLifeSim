#include "particle.h"
#include "raymath.h"

//=========================================================================================
particle::particle(Vector3 p, Vector3 v, int c) 
: pos(p), velocity(v), color(c){
    int x = -10;
    int y = -10;
    int z = -10;
    this->currentHash = std::make_tuple(x, y, z);
    this->force = Vector3Zero();
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
    //DrawSphereEx(this->pos, 0.08f, 2, 2, col);
    DrawCircle3D(this->pos, 0.03f, Vector3{0.0, 1.0, 0.0}, 90.0f, col);

}

//=========================================================================================
//Updates the position, velocity, etc for the current particle.
void particle::updateParticle(float time){
    float maxVelocity = 3.0f;
    this->velocity = Vector3Add(this->velocity, this->force);
    if(this->velocity.x > maxVelocity){
        this->velocity.x = maxVelocity;
    }
    if(this->velocity.y > maxVelocity){
        this->velocity.y = maxVelocity;
    }
    if(this->velocity.z > maxVelocity){
        this->velocity.z = maxVelocity;
    }
    if(this->velocity.x < -maxVelocity){
        this->velocity.x = -maxVelocity;
    }
    if(this->velocity.y < -maxVelocity){
        this->velocity.y = -maxVelocity;
    }
    if(this->velocity.z < -maxVelocity){
        this->velocity.z = -maxVelocity;
    }
   
   
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
//Determines force based on (other) particle p (within radius).
Vector3 particle::colorInteraction(const particle& p, const float (&attrM)[6][6]){
    float scaleAmount = 0.2f, minDistance = 20.0f, force;
    Vector3 directionVec, nDirection, forceVec;
    directionVec = Vector3Subtract(p.pos, this->pos);
    float distance = Vector3Length(directionVec) + 0.05f;
    nDirection = Vector3Normalize(directionVec);
    float scaledAttraction = attrM[this->color - 1][p.color - 1] * scaleAmount;
    if (distance <= minDistance) {
        force = -scaledAttraction * (minDistance / distance - 1);
    } else {
        force = scaledAttraction;
    }

    return forceVec = Vector3Scale(nDirection, force);
}


//=========================================================================================
//Operator overload for == between particle objs.
bool particle::operator==(particle const& right){
    return (this->id == right.id);
}