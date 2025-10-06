#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D uScreenTexture;

void main()
{
    FragColor = vec4(vec3(1.0 - texture(uScreenTexture, TexCoords)), 1.0);
}
