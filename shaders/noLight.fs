#version 330 core
out vec4 FragColor;
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
    bool hasNormalMap;
    bool hasSpecularMap;
    bool hasDiffuseMap;
    vec3 color;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

uniform Material material;

void main() {
    if(material.hasDiffuseMap)
        FragColor = vec4(texture(material.diffuse, fs_in.TexCoords));
    else
        FragColor = vec4(material.color, 1.0);
}