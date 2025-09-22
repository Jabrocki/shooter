#version 410 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

layout(std140) uniform Globals
{
    mat4 uView;
    mat4 uProjection;
    vec3 uViewPosition;
};

//Material
struct Material {
    sampler2D diffuse0;
    sampler2D specular0;
    float shininess;
};

uniform Material uMaterial;

//Lights

//Point lights
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define LIGHT_COUNT 5
uniform Light uLights[LIGHT_COUNT];

//Directional lights
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


//Consts 
float near = 0.1;
float far = 100.0;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(uMaterial.diffuse0, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(uMaterial.diffuse0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular0, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(uMaterial.diffuse0, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(uMaterial.diffuse0, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(uMaterial.specular0, TexCoord));

    return (ambient + diffuse + specular);
}  

// Output color
out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(uViewPosition - FragPos);

    // Dir light for dwbbuging
    DirLight dirLight;
    dirLight.direction = vec3(0.0, 0.0, -1.0);
    dirLight.ambient = vec3(0.5, 0.5, 0.5);
    dirLight.diffuse = vec3(0.6, 0.6, 0.6);
    dirLight.specular = vec3(0.8, 0.8, 0.8);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // Calc point Light
    for (int i = 0; i < LIGHT_COUNT; i++)
        result += CalcPointLight(uLights[i], norm, FragPos, viewDir);

    // Future: calc spotlight
    FragColor = vec4(result, 1.0);
}