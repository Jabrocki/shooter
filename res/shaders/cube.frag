#version 410 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


//Globals
layout(std140) uniform Globals
{
    mat4 uView;
    mat4 uProjection;
    vec3 uViewPosition;
};

//Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material uMaterial;

//Light
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light uLight;

//Texture
uniform sampler2D uTexture;




void main() {

    vec3 ambient = uLight.ambient * uMaterial.ambient;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);

    vec3 viewDir = normalize(uViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * (spec * uMaterial.specular);

    vec3 light = ambient + diffuse + specular;
    vec4 result = vec4(light, 1.0);

    FragColor = texture(uTexture, TexCoord) * result;
}