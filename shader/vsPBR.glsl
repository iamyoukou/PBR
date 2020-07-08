#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    TexCoords = aTexCoords;
    WorldPos = vec3(M * vec4(aPos, 1.0));
    Normal = mat3(M) * aNormal;

    gl_Position =  P * V * vec4(WorldPos, 1.0);
}
