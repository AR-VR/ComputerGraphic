//
//  Shape.hpp
//  Tutorials
//
//  Created by Xiang Guo on 2016-02-21.
//
//

#ifndef Shape_hpp
#define Shape_hpp

#include <stdio.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>

class Shape
{
public:
    const std::vector<GLfloat>& getBuffer() const;
    Shape();
private:
    std::vector<GLfloat> vertexBuffer;
};
#endif /* Shape_hpp */
