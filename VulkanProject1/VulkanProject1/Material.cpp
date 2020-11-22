#include "Material.h"

Material::Material() :specularIntensity(0.0f), shininess(0.0f)
{
}

Material::Material(float specIntensity, float shiny):specularIntensity(specIntensity), shininess(shiny)
{
}
