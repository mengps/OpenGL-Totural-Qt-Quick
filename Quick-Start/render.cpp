#include "render.h"

void Render::initializeGL()
{
    initializeOpenGLFunctions();
    initializeShader();
}

void Render::initializeShader()
{

}

void Render::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Render::render()
{

}
