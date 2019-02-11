#version 330 core														

in vec3 positionInWorld;
in vec3 transformNormal;
in vec2 texCoordToFragment;

uniform vec3 lightPosition;
uniform vec3 spotLightColor;

uniform vec3 cameraPosition;

uniform sampler2D fragmentTexture;

out vec4 FragColor;

void main() {
  //light reflection is reverse of initial light direction
  vec3 lightDirection = normalize(positionInWorld-lightPosition);
  vec3 reflectDirection = reflect(lightDirection, transformNormal);
  vec3 viewDirection = normalize(cameraPosition - positionInWorld);
  float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), 2.0);
  FragColor = texture(fragmentTexture, texCoordToFragment)*vec4(specularIntensity*spotLightColor, 1.0);
}