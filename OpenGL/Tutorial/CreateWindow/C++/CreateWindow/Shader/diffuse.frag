#version 330 core														

in vec3 positionInWorld;
in vec3 transformNormal;

uniform vec3 lightPosition;
uniform vec3 diffuseLightColor;

out vec4 FragColor;

void main() {
  //light reflection is reverse of initial light direction
  vec3 reflectLight = -normalize(positionInWorld-lightPosition);
  float diffuseIntensity = max(dot(transformNormal, reflectLight), 0.0);
  FragColor = vec4(diffuseLightColor*diffuseIntensity, 1.0);
}