#ifndef PARTICLE
#define PARTICLE
#include "raylib.h"
#include <iostream>
#include <tuple>
#include <algorithm>

class particle{
    public:
        particle(Vector3 p, Vector3 v, int c);
        ~particle(){};
        void drawParticle();
        void updateParticle(float time);
        Vector3 colorInteraction(const particle& p, const float (&attrM)[6][6]);
        bool operator==(particle const& right);
        Vector3 pos;
        Vector3 velocity;
        Vector3 force;
        int color = -1;
        int id = -1;
        std::tuple <int, int, int> currentHash;
        
};


#endif