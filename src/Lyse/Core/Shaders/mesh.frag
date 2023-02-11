#version 330 core

in vec3 pos;
in vec3 normal;

uniform vec3 cameraPos;

out vec4 FragColor;

void main()
{
    vec3 lightDir = -vec3(1.0, 1.0, 1.0);
    vec3 uLightDir = normalize(lightDir);
    vec3 uNormal = normalize(normal);
    vec3 baseColor = vec3(1.0, 1.0, 1.0);

    float ambiant = 0.05;
    float diffuse = 0.6*clamp(dot(-uLightDir, uNormal), 0.0, 1.0);
    
    vec3 reflection = reflect(uLightDir, uNormal);
    float specular = 0.5*pow(clamp(dot(reflection, normalize(cameraPos - pos)), 0.0, 1.0), 100.0);

    FragColor = vec4(baseColor * (ambiant + diffuse + specular), 1.0);
} 
