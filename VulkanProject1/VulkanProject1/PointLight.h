#pragma once
#include <glm/glm.hpp>
class PointLight
{
public:
	PointLight();
	PointLight(glm::vec3 pos, glm::vec3 col ,float cons, float lin, float ex, float aIntensity, float dIntensity);

	static const int POINT_LIGHT_COUNT = 1;

	inline glm::vec3 getPosition() { return position; }
	inline float getConstant() { return constant; }
	inline float getLinear() { return linear; }
	inline float getExponent() { return exponent; }

	inline float getAmbientIntensity() { return ambientIntensity; }
	inline float getDiffuseIntensity() { return diffuseIntensity; }
	inline glm::vec3 getColor() { return color; }
protected:
	glm::vec3 position;
	glm::vec3 color;
	float constant;
	float linear;
	float exponent;
	float ambientIntensity;
	float diffuseIntensity;
};

