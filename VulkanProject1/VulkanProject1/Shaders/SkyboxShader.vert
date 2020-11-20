#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
}ubo;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragTexCoord;

void main()
{
    mat4 view = mat4(mat3(ubo.view));
    gl_Position = ubo.proj * view * vec4(inPosition, 1.0);
    fragTexCoord = inPosition;
}