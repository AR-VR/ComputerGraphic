#version 330 core
//This define position variable at vertex attribute pointer index 0
layout (location = 0) in vec3 position;
void main() {
  //Convert vec3 position to vec4 position (eventually for w division?)
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
}