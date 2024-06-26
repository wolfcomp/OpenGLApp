#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform vec3 viewPos;

out VS_OUT {
   vec3 FragPos;
   vec2 TexCoords;
   vec3 TangentViewPos;
   vec3 TangentFragPos;
   vec3 Normal;
   vec4 FragPosLightSpace;
   mat3 TBN;
} vs_out;

void main() {
   vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
   vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
   vs_out.TexCoords = aTexCoords;
   mat3 normalMatrix = transpose(inverse(mat3(model)));
   vec3 T = normalize(normalMatrix * aTangent);
   vec3 N = normalize(normalMatrix * aNormal);
   T = normalize(T - dot(T, N) * N);
   vec3 B = cross(N, T);
   mat3 TBN = mat3(T, B, N);
   vs_out.TBN = TBN;
   vs_out.TangentViewPos = TBN * viewPos;
   vs_out.TangentFragPos = TBN * vs_out.FragPos;
   vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
   gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}