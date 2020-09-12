#pragma once
#include <vector>

class Triangle
{
public:
	Triangle();
	const std::vector<float> ForLine() { return verticesForLine; }
	const std::vector<float> ForShape() { return verticesForShape; }
	const std::vector<float> TextureCoord() { return textureCoords; }
private:
	std::vector<float> verticesForLine;
	std::vector<float> verticesForShape;
	std::vector<float> textureCoords;
};

