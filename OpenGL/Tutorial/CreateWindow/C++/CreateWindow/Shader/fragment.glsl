#version 330 core														
out vec4 FragColor;	
in vec3 colorToFragment;												
void main() {							
  FragColor = vec4(colorToFragment, 1.0f);	
}