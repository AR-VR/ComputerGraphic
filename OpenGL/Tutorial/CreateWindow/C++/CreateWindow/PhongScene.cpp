#include "PhongScene.h"
#include "GLErrorCheck.h"

void PhongScene::Draw(Camera& camera, Shape& shape, Shader& program)
{
  GL_EXEC(glEnable(GL_DEPTH_TEST));
  GL_EXEC(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GL_EXEC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // also clear the depth buffer now!
  
  program.UseProgram();
  GL_EXEC(glBindVertexArray(GetVertexArrayObject()));
  GL_EXEC(glActiveTexture(GL_TEXTURE0));
  GL_EXEC(glBindTexture(GL_TEXTURE_2D, GetTexture()));
  program.SetUniformMatrix4fv("model", glm::transpose(shape.GetModelMatrix()));
  program.SetUniformMatrix4fv("view", glm::transpose(camera.GetViewMatrix()));
  program.SetUniformMatrix4fv("projection", glm::transpose(camera.GetProjectionMatrix()));
  //https://github.com/AR-VR/ComputerGraphic/wiki/Transformation-of-Surface-Normal
  glm::mat4 normalTransform(glm::transpose(glm::inverse(camera.GetViewMatrix()*shape.GetModelMatrix())));
  program.SetUniformMatrix4fv("normalTransform", glm::transpose(normalTransform));
  glm::vec3 lightPosition(1, 1, 1);
  program.SetUniform3fv("lightPosition", lightPosition);

  program.SetUniform3fv("cameraPosition", camera.GetCameraPosition());
  program.SetUniform3fv("lightColor", glm::vec3(1, 1, 1));
  program.SetUniformFloat("ambientStrength", 0.9f);
  program.SetUniform3fv("spotLightColor", glm::vec3(0, 1, 1));
  program.SetUniform3fv("ambientLightColor", glm::vec3(1, 1, 1));
  
  GL_EXEC(glDrawArrays(GL_TRIANGLES, 0, shape.GetVerticesCountTriangle()));
  GL_EXEC(glBindVertexArray(0));
}