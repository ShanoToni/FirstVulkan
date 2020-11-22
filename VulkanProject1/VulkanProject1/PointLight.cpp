#include "PointLight.h"

PointLight::PointLight() : position(glm::vec3(0.0f)), constant(1.0f), linear(0.0f), exponent(0.0f), ambientIntensity(1.0f), diffuseIntensity(0.0f)
{
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 col, float cons, float lin, float ex, float aIntensity, float dIntensity) 
	: position(pos), color(col), constant(cons), linear(lin), exponent(ex), ambientIntensity(aIntensity), diffuseIntensity(dIntensity)
{
}
