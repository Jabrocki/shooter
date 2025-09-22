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
    sampler2D diffuse0;
    sampler2D specular0;
    float shininess;
};

uniform Material uMaterial;

//Light
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light uLight;


float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}



void main() {
    //Ambient
    vec3 ambient = uLight.ambient * vec3(texture(uMaterial.diffuse0, TexCoord));
    
    //Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = uLight.diffuse * diff * vec3(texture(uMaterial.diffuse0, TexCoord));

    //Specular
    vec3 viewDir = normalize(uViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = uLight.specular * spec * vec3(texture(uMaterial.specular0, TexCoord));

    //Calc distance
    float dist = length(uLight.position - FragPos);
    float attenutaion = 1.0 / (uLight.constant + uLight.linear * dist + uLight.quadratic * (dist * dist));

    ambient *= attenutaion;
    diffuse *= attenutaion;
    specular *= attenutaion;

    vec3 light = ambient + diffuse + specular;
    vec4 result = vec4(light, 1.0);

    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);
    FragColor = result;
}