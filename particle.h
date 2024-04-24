#ifndef PARTICLE
#define PARTICLE
#include "raylib.h"

class particle{
    public:
        particle(Vector3 p, Vector3 v, int c);
        ~particle(){};
        void drawParticle();
        void updateParticle(float time);
        void colorInteraction(const particle& p, const float (&attrM)[6][6]);
        bool operator==(particle const& right);
        Vector3 pos;
        Vector3 velocity;
        const int color;
        
};


#endif