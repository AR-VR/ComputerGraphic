#version 330 core														
uniform vec3 lightColor;

out vec4 FragColor;	

void main() {							
  //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); 
  FragColor = lightColor; 
}