#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : direction(glm::vec3(0.0f, 1.0f, 0.0f)), color(glm::vec3(1.0f)), ambientIntensity(1.0f), diffuseIntensity(0.0f)
{
}

DirectionalLight::DirectionalLight(glm::vec3 col, float aIntensity, float dIntensity, glm::vec3 dir) : direction(dir), color(col), ambientIntensity(aIntensity), diffuseIntensity(dIntensity)
{
}
