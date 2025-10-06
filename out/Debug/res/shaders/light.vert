#version 410 core
layout(location = 0) in vec3 aPos;

//Globals
layout(std140) uniform Globals
{
    mat4 uView;
    mat4 uProjection;
    vec3 uViewPosition;
};

uniform mat4 uModel;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}

