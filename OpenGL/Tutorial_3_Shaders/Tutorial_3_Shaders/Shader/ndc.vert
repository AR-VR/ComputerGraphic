#version 330 core
//This define position variable at vertex attribute pointer index 0
layout (location = 0) in vec3 inPosition;

void main() 
{
  //Convert vec3 position to vec4 position (eventually for w division?)
  gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
}