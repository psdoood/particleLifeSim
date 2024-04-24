#ifndef PARTICLE
#define PARTICLE
#include "raylib.h"

class particle{
    private:
        Vector3 pos;
        Vector3 velocity;
        Color color;
    public:
        particle(Vector3 p, Vector3 v, Color c);
        ~particle(){};
        void drawParticle();
        void updateParticle(float time);
        void colorInteraction(const particle& p);
};


#endif