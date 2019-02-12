#version 330 core														

in vec3 positionInWorld;
in vec3 transformNormal;
in vec2 texCoordToFragment;

uniform vec3 lightPosition;
uniform vec3 spotLightColor;
uniform vec3 ambientLightColor;
uniform vec3 cameraPosition;

uniform sampler2D fragmentTexture;

out vec4 FragColor;

void main() {
  //light reflection is reverse of initial light direction
  vec3 lightDirection = normalize(positionInWorld-lightPosition);
  vec3 reflectDirection = normalize(reflect(lightDirection, transformNormal));
  vec3 viewDirection = normalize(cameraPosition - positionInWorld);
  
  //Ambient Light
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength*ambientLightColor;
  
  //Diffuse Light
  float diffuseIntensity = max(dot(transformNormal, -lightDirection), 0.0);
  vec3 diffuse = diffuseIntensity*spotLightColor;

  //Specular Light
  float specularStrength = 0.5;
  //2.0 is shininess of the object
  float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), 2.0);
  vec3 specular = specularStrength*specularIntensity*spotLightColor;


  
  FragColor = texture(fragmentTexture, texCoordToFragment)*vec4(ambient+diffuse+specular, 1.0);
}