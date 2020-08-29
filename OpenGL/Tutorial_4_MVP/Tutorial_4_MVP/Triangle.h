#pragma once
#include <vector>

class Triangle
{
public:
	Triangle();
	const std::vector<float> ForLine() { return verticesForLine; }
	const std::vector<float> ForShape() { return verticesForShape; }

private:
	std::vector<float> verticesForLine;
	std::vector<float> verticesForShape;
};

