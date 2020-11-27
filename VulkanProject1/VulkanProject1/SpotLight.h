#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();
    SpotLight(glm::vec3 col, glm::vec3 pos, glm::vec3 dir, float aIntensity, float dIntensity, float cons, float lin, float exp, float edg);

    static const int SPOT_LIGHT_COUNT = 2;
    
    inline float getAmbientIntensity() { return ambientIntensity; }
    inline float getDiffuseIntensity() { return diffuseIntensity; }
    inline glm::vec3 getDirection() { return direction; }
    inline float getEdge() { return edge; }
    inline float getprocEdge() { return procEdge;}
private:
    glm::vec3 direction;
    float edge;
    float procEdge;
};

