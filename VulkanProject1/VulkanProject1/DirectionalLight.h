#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 col, float aIntensity, float dIntensity, glm::vec3 dir);

	inline glm::vec3 getDirection() { return direction; }
	inline glm::vec3 getColor() { return color; }
	inline float getAmbientIntensity() { return ambientIntensity; }
	inline float getDiffuseIntensity() { return diffuseIntensity; }
private:
	glm::vec3 direction;
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

