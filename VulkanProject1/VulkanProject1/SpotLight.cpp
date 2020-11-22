#include "SpotLight.h"

SpotLight::SpotLight()
{
}

SpotLight::SpotLight(glm::vec3 col, glm::vec3 pos, glm::vec3 dir, float aIntensity, float dIntensity, float cons, float lin, float exp, float edg)
{
	color = col;
	position = pos;
	direction = dir;
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
	constant = cons;
	linear = lin;
	exponent = exp;
	edge = edg;
	procEdge = cosf(glm::radians(edge));
}
