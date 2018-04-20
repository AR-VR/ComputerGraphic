#version 330 core														
in vec2 texCoordToFragment;

uniform sampler2D fragmentTexture;

uniform vec3 lightColor;

uniform float ambientStrength;

out vec4 FragColor;	

void main() {							
  vec3 ambientLight = ambientStrength*lightColor;
  FragColor = texture(fragmentTexture, texCoordToFragment)*vec4(ambientLight, 1.0f); 
}