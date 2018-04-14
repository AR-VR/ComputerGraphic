#version 330 core
//This define position variable at vertex attribute pointer index 0
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 colorToFragment;
out vec2 texCoordToFragment;

void main() 
{
  //Convert vec3 position to vec4 position (eventually for w division?)
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  colorToFragment = inColor;
  texCoordToFragment = vec2(inTexCoord.x, inTexCoord.y);
}