#ifndef RENDER_H
#define RENDER_H
#include <QOpenGLFunctions>

class Render : protected QOpenGLFunctions
{

public:
    Render() { }
    virtual ~Render() { }

public:
    virtual void initializeGL();
    virtual void initializeShader();

    virtual void resizeGL(int w, int h);
    virtual void render();    //与paintGL相似，但我更喜欢叫render
};

#endif // RENDER_H
