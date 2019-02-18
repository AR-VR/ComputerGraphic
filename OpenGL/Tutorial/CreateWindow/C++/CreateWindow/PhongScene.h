#pragma once

#include "Scene.h"

class PhongScene : public Scene
{
public:
  void Draw(Camera& camera, Shape& shape, Shader& program);
};

