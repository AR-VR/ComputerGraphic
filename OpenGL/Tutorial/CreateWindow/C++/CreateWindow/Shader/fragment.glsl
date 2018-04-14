#version 330 core														
in vec3 colorToFragment;
in vec2 texCoordToFragment;

uniform sampler2D fragmentTexture;

out vec4 FragColor;	

void main() {							
  FragColor = texture(fragmentTexture, texCoordToFragment)*vec4(colorToFragment, 1.0f); 
}