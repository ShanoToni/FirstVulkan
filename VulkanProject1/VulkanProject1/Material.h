#pragma once
class Material
{
public:
	Material();
	Material(float specIntensity, float shiny);

	inline float getSpecularIntensity() { return specularIntensity; }
	inline float getShininess() { return shininess; }

private:
	float specularIntensity;
	float shininess;
};

