#pragma once
#include <vector>

class Triangle
{
public:
	Triangle();
	const float * const ForLine() { return verticesForLine.data(); }
	const float * const ForShape() { return verticesForShape.data(); }

private:
	std::vector<float> verticesForPoint;
	std::vector<float> verticesForLine;
	std::vector<float> verticesForShape;
};

