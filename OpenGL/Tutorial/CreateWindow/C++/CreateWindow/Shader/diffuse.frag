#version 330 core														

in vec3 positionInWorld;
in vec3 transformNormal;
in vec2 texCoordToFragment;

uniform vec3 lightPosition;
uniform vec3 diffuseLightColor;
uniform sampler2D fragmentTexture;

out vec4 FragColor;

void main() {
  //light reflection is reverse of initial light direction
  vec3 reflectLight = -normalize(positionInWorld-lightPosition);
  //Light color can not be negative, minimum is 0 (black)
  float diffuseIntensity = max(dot(transformNormal, reflectLight), 0.0);
  FragColor = texture(fragmentTexture, texCoordToFragment)*vec4(diffuseLightColor*diffuseIntensity, 1.0);
}