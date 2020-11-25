#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 fragPos;

layout(binding = 1) uniform sampler2D texSampler;

struct directionalLightStruct {
    vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 direction;
};

struct PointLightStruct
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLightStruct
{
	PointLightStruct base;
	vec3 direction;
	float edge;
};

struct MaterialStruct
{
	float specularIntensity;
	float shininess;
};

layout(binding = 2) uniform DirectionalLight{
    directionalLightStruct base;
}dLight;

layout(binding = 3) uniform PointLight{
    PointLightStruct base;
}pLight;

layout(binding = 4) uniform SpotLight{
    SpotLightStruct base;
}sLight;

layout(binding = 5) uniform Material{
    MaterialStruct base;
}mat;

layout(binding = 6) uniform CamPos{
    vec3  pos;
}camPos;


layout(location = 0) out vec4 outColor;

vec4 CalcLightByDirection( vec3 color,float ambientIntensity,float diffuseIntensity, vec3 direction)
{
	vec4 ambientColor = vec4(color, 1.0f) * ambientIntensity;
	//base off of the angle of the light towards a surface
	vec3 fix = vec3(-direction.x, -direction.y, -direction.z);
	float diffuseFactor = max(dot(normalize(normals), normalize(fix)), 0.0f);
	//diffuse
	vec3 diffCol = color * diffuseIntensity * diffuseFactor;
	vec4 diffuseColor = vec4(diffCol, 1.0f);

	//specular
	vec4 specularColor = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(camPos.pos - fragPos);
		vec3 reflectedVert = normalize(reflect(direction, normalize(normals)));

		float specFactor = dot(fragToEye, reflectedVert);

		if(specFactor> 0.0f)
		{
			specFactor = pow(specFactor, mat.base.shininess);
			specularColor = vec4(color * diffuseIntensity * mat.base.specularIntensity * specFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(dLight.base.color, dLight.base.ambientIntensity, dLight.base.diffuseIntensity , dLight.base.direction);
}

void main()
{
    //outColor = vec4(fragTexCoord, 0.0, 1.0);
    outColor = vec4((texture(texSampler, fragTexCoord) * CalcDirectionalLight()).xyz, 1.0);
    //outColor = vec4(dLight.base.direction , 1.0);
}