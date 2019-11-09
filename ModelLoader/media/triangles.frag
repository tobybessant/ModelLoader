#version 450 core

in vec4 fragColour;
in vec2 TexCoord;

out vec4 fColor;

uniform sampler2D ourTexture;

void main()
{
	fColor = texture(ourTexture, TexCoord);	
}
