#version 330 core
//This define position variable at vertex attribute pointer index 0
layout (location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
  //Convert vec3 position to vec4 position (eventually for w division?)
  gl_Position = projection * view * model * vec4(inPosition, 1.0);
}