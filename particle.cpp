#include "particle.h"


particle::particle(Vector3 p, Vector3 v, Color c) 
: pos(p), velocity(v), color(c){}

//Draws the particle based on its current position.
void particle::drawParticle(){

}

//Updates the position, velocity, etc for the current particle.
void particle::updateParticle(){

}

//Determines how (this) particle should react based on particle p.
void particle::colorInteraction(const particle& p){

}