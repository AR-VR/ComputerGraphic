#version 330 core														
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 positionInWorld;
out vec3 transformNormal;

void main() {
  //Model matrix will change vertex position in world
  positionInWorld = vec3(model * vec4(inPosition, 1.0));

  //https://github.com/AR-VR/ComputerGraphic/wiki/Transformation-of-Surface-Normal
  transformNormal  = mat3(transpose(inverse(view*model)))*inNormal;
  gl_Position = projection * view * vec4(positionInWorld, 1.0);
}