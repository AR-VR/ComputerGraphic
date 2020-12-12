#include "Triangle.h"

Triangle::Triangle()
{
	//   1
	//   * *
	//   *   *
	//   0 * * 2
	verticesForShape =
	{
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	//GL_LINES: Vertices 0 and 1 are considered a line. Vertices 2 and 3 are considered a line. And so on. If the user specifies a non-even number of vertices, then the extra vertex is ignored.
	verticesForLine = {
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,

		-0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	verticesWithTexCoord = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f
	};
	//   1
	//   * *
	//   *   *
	//   0 * * 2
	textureCoords = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	//GL_LINE_STRIP: The adjacent vertices are considered lines. Thus, if you pass n vertices, you will get n-1 lines. If the user only specifies 1 vertex, the drawing command is ignored.
	//verticesForLine = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f,  0.5f, 0.0f,
	//	-0.5f, -0.5f, 0.0f,
	//};
	
	//GL_LINE_LOOP: As line strips, except that the first and last vertices are also used as a line. Thus, you get n lines for n input vertices. If the user only specifies 1 vertex, the drawing command is ignored. The line between the first and last vertices happens after all of the previous lines in the sequence.
	//verticesForLine = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f,  0.5f, 0.0f,
	//};
}

