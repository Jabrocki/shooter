// pp_invert.frag
#version 330 core
out vec4 FragColor;

in vec2 vUV;
uniform sampler2D uScreenTexture; // must be bound to a texture unit once

void main() {
    vec3 c = texture(uScreenTexture, vUV).rgb;
    FragColor = vec4(1.0 - c, 1.0);
}
