#version 410 core
out vec4 FragColor;


//Globals
layout(std140) uniform Globals
{
    mat4 uView;
    mat4 uProjection;
    vec3 uViewPosition;
    vec3 uLightPosition;
};

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0
}
