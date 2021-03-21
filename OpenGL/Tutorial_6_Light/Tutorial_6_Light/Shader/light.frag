#version 330 core

in vec2 texCoordToFragment;

uniform sampler2D fragmentTexture;

out vec4 FragColor;

void main()
{
    FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f)*texture(fragmentTexture, texCoordToFragment);
	//FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
} 