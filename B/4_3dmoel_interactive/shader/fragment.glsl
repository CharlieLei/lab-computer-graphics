#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 color;
};

struct Light {
    vec3 position;
    vec3 intensity;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

void main() {
    vec3 ambient = material.ambient;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max( dot(lightDir, normal), 0.0 );
    vec3 diffuse = diff * material.diffuse * light.intensity;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow( max(dot(reflectDir, viewDir), 0.0), material.shininess );
    vec3 specular = spec * material.specular * light.intensity;

    vec3 result = (ambient + diffuse + specular) * material.color;

    FragColor = vec4(result, 1.0);
}
