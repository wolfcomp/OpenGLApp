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

void main() {
    FragColor = vec4(fs_in.TexCoords, 0, 1.0);
}