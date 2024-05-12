#version 330 core
out vec4 FragColor;
in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

uniform vec3 viewPos;

void main() {
    vec3 dir = fs_in.FragPos - viewPos;
    dir = normalize(dir);

    vec3 absNormal = abs(fs_in.Normal);
    float facing = dot(fs_in.Normal, normalize(dir)) * 0.5 + 0.5;

    FragColor = vec4(facing * absNormal, 1.0);
}