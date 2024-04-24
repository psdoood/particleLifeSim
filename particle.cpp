#include "particle.h"


particle::particle(Vector3 p, Vector3 v, Color c) 
: pos(p), velocity(v), color(c){}

//Draws the particle based on its current position.
void particle::drawParticle(){
    DrawSphereEx(this->pos, 0.08f, 3, 3, this->color);
}

//Updates the position, velocity, etc for the current particle.
void particle::updateParticle(float time){
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

//Determines how (this) particle should react based on particle p.
void particle::colorInteraction(const particle& p){

}