#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//Globals
layout(std140) uniform Globals
{
    mat4 uView;
    mat4 uProjection;
    vec3 uViewPosition;
};

uniform mat4 uModel;

out vec2 TexCoord;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord = aTexCoord;
}