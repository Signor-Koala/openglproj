#version 460 core
in vec4 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct material {
    sampler2D diffuse;
    sampler2D specular;
    float reflectivity;
};

uniform material Material;
uniform vec3 lightSourceColor;
uniform vec3 lightAmbientColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 ambient = lightAmbientColor * vec3(texture(Material.diffuse, TexCoords));

    vec3 norm = normalize(vec3(Normal));
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightSourceColor * diff * vec3(texture(Material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Material.reflectivity);
    vec3 specular = lightSourceColor * spec * vec3(texture(Material.specular, TexCoords));

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
}