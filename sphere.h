//
// Created by Yan Yu on 4/23/22.
//

#ifndef SPHERE_H
#define SPHERE_H

//#include <GL/glew.h>
#include <glad/glad.h>

class Sphere
{
public:
    Sphere();
    ~Sphere();
    void init(GLuint vertexPositionID);
    void cleanup();
    void draw();

private:
    int lats, longs;
    bool isInited;
    GLuint m_vao, m_vboVertex, m_vboIndex;
    int numsToDraw;
};

#endif // SPHERE_H