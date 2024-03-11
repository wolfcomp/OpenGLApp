#version 330 core
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;

// ambient light properties
uniform vec3 ambientColor;

// light properties
uniform vec3 lightColor;
uniform vec3 lightPos;

// material properties
uniform float shininess;
uniform float specular;
uniform float metallic;
uniform vec3 albedo;

// camera position and orientation
uniform vec3 camPos;
uniform mat4 camView;

void main() {
    vec3 lightDir = normalize(lightPos - vec3(0.0, 0.0, 0.0));
    vec3 viewPos = -normalize(vec3(camView * vec4(normalize(fragPos - camPos), 1)));
    vec3 reflectDir = reflect(-lightDir, vec3(normal));

    float spec = pow(max(dot(viewPos, reflectDir), 0.0), shininess);
    vec3 specular = lightColor * spec * specular;

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 difuse = lightColor * diff;

    vec3 result = (ambientColor + difuse + specular) * albedo;

    FragColor = vec4(result, 1.0);
}