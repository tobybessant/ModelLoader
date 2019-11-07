#version 450 core

out vec4 fColor;
in vec4 fragColour;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    fColor = vec4(1.0, 0.1, 0.6, 0.4);
	//fColor = texture(ourTexture, TexCoord) * fragColour;
	
}
