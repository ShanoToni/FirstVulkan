#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 fragPos;

layout(binding = 1) uniform sampler2D texSampler;

const int MAX_POINT_LIGHT = 3;
const int MAX_SPOT_LIGHT = 2;


struct MaterialStruct
{
	float specularIntensity;
	float shininess;
};
struct directionalLightStruct {
	float ambientIntensity;
	float diffuseIntensity;
    vec4 color;
	vec4 direction;
};

struct PointLightStruct
{
	float ambientIntensity;
	float diffuseIntensity;
	float constant;
	float linear;
	float exponent;
	vec4 color;
	vec4 position;
};

struct SpotLightStruct
{
	float ambientIntensity;
	float diffuseIntensity;
	float constant;
	float linear;
	float exponent;
	float edge;
	vec4 color;
	vec4 position;
	vec4 direction;
};


layout(binding = 2) uniform DirectionalLight{
	directionalLightStruct base;
}dLight;

layout(binding = 3) uniform PointLight{
   	PointLightStruct base[MAX_POINT_LIGHT];
}pLight;

layout(binding = 4) uniform SpotLight{
    SpotLightStruct base[MAX_SPOT_LIGHT];
}spLight;

layout(binding = 5) uniform Material{
    MaterialStruct base;
}mat;

layout(binding = 6) uniform CamPos{
    vec3  pos;
}camPos;


layout(location = 0) out vec4 outColor;

vec4 CalcLightByDirection( vec4 color,float ambientIntensity,float diffuseIntensity, vec4 direction)
{
	vec4 ambientColor = color * ambientIntensity;
	//base off of the angle of the light towards a surface
	vec3 fix = vec3(-direction.x, -direction.y, -direction.z);
	float diffuseFactor = max(dot(normalize(normals), normalize(fix)), 0.0f);
	//diffuse
	vec3 diffCol = color.xyz * diffuseIntensity * diffuseFactor;
	vec4 diffuseColor = vec4(diffCol, 1.0f);

	//specular
	vec4 specularColor = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f)
	{

		vec3 fragToEye = normalize(camPos.pos - fragPos.xyz);
		vec3 halfwayDir = normalize(-direction.xyz + fragToEye);

		vec3 reflectedVert = normalize(reflect(direction.xyz, normalize(normals)));

		//float specFactor = dot(fragToEye, reflectedVert);
		float specFactor = max(dot(normals, halfwayDir), 0.0f);

		if(specFactor > 0.0f)
		{
			specFactor = pow(specFactor, mat.base.shininess);
			specularColor = vec4(color.xyz * diffuseIntensity * mat.base.specularIntensity * specFactor, 1.0f);
		}
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(vec4 color,	float ambientIntensity,	float diffuseIntensity,	vec4 position, float constant,	float linear, float exponent)
{
	vec3 direction = fragPos - position.xyz;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 pointLightColor = CalcLightByDirection(color, ambientIntensity, diffuseIntensity, vec4(direction,1.0f));

	float attenuation = exponent * distance * distance +
						linear * distance +
						constant;
	
	return (pointLightColor/attenuation);
}

vec4 calcSpotLight(float ambientIntensity, float diffuseIntensity, float constant, float linear, float exponent, float edge, vec4 color, vec4 position, vec4 direction)
{
	vec3 rayDirection = normalize(fragPos - position.xyz);
	float slFactor = dot(rayDirection, direction.xyz);

	//if in cone
	if(slFactor > edge)
	{
		vec4 spotColor = calcPointLight(color, ambientIntensity, diffuseIntensity,position, constant, linear, exponent);
		
		return spotColor * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - edge)));
		//return spotColor ;
	}
	else
	{
		return vec4(0,0,0,1);
	}
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(dLight.base.color, dLight.base.ambientIntensity, dLight.base.diffuseIntensity , dLight.base.direction);
}

vec4 calcPointLights()
{
	vec4 totalCol = vec4(0,0,0,1);
	for(int i = 0; i< MAX_POINT_LIGHT; i++)
	{
		totalCol += calcPointLight(pLight.base[i].color, pLight.base[i].ambientIntensity, pLight.base[i].diffuseIntensity, pLight.base[i].position, pLight.base[i].constant, pLight.base[i].linear, pLight.base[i].exponent);
	}
	return  totalCol;
}

vec4 calcSpotLights()
{
	vec4 totalCol = vec4(0,0,0,1);
	for(int i = 0; i< MAX_SPOT_LIGHT; i++)
	{
		totalCol += calcSpotLight(spLight.base[i].ambientIntensity, spLight.base[i].diffuseIntensity, spLight.base[i].constant, spLight.base[i].linear, spLight.base[i].exponent, spLight.base[i].edge, spLight.base[i].color, spLight.base[i].position, spLight.base[i].direction);
	}
	return  totalCol;
}


void main()
{
	float gamma = 2.2f;

    //outColor = vec4(fragTexCoord, 0.0, 1.0);

	vec4 lightColor = vec4(0,0,0,1);
	lightColor = CalcDirectionalLight();
	lightColor = lightColor + calcPointLights();
	lightColor = lightColor + calcSpotLights();

    outColor = vec4(((texture(texSampler, fragTexCoord) * lightColor).xyz), 1.0);

    //outColor = vec4(dLight.base.direction , 1.0);
}