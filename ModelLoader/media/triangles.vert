
#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aColour;

uniform mat4 mvp;

out vec2 TexCoord;
out vec3 Normal;
out vec4 fragColour;

void main()
{
    gl_Position = mvp * vec4(aPos,1.0);
	TexCoord = aTexCoords;
	Normal = aNormal;
	fragColour = aColour;
}
