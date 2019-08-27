#version 450 core														
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//normalTransform = mat3(transpose(inverse(view*model)))
//https://github.com/AR-VR/ComputerGraphic/wiki/Transformation-of-Surface-Normal
uniform mat4 normalTransform; 


out vec3 positionInWorld;
out vec3 transformNormal;
out vec2 texCoordToFragment;

void main() {
  //Model matrix will change vertex position in world
  positionInWorld = vec3(model * vec4(inPosition, 1.0));

  //Normalization is needed as we only want to know direction
  transformNormal = normalize(mat3(normalTransform)*inNormal);

  gl_Position = projection * view * vec4(positionInWorld, 1.0);
  texCoordToFragment = inTexCoord;
}