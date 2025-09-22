#version 410 core

in vec2 TexCoord;

layout(std140) uniform Globals
{
    mat4 uView;
    mat4 uProjection;
    vec3 uViewPosition;
};

struct Material {
    sampler2D diffuse0;
};

uniform Material uMaterial;

out vec4 FragColor;

void main()
{
    FragColor = texture(uMaterial.diffuse0, TexCoord);
}