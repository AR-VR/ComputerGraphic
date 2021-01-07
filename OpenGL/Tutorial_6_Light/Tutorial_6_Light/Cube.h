#pragma once
#include<vector>

class Cube
{
public:
	Cube();
	const std::vector<float> ForShape() { return verticesForShape; }
	const std::vector<float> ShapeWithTexture() { return verticesWithTexCoord; }

private:
	std::vector<float> verticesForShape;
	std::vector<float> verticesWithTexCoord;
};

