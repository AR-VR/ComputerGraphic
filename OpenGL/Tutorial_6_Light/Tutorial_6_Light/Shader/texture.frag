#version 330 core

in vec2 texCoordToFragment;

uniform sampler2D fragmentTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(fragmentTexture, texCoordToFragment);
	//FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
} 