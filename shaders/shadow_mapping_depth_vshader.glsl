#version 330 core
in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;
in vec2 vTexture;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(vPosition, 1.0);
    texCoord = vTexture;
}
