#ifndef TRIANGLERENDER_H
#define TRIANGLERENDER_H
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class TriangleRender : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    TriangleRender();
    ~TriangleRender();

    void initializeGL();   

public slots:
    void render();
    void resizeGL(int w, int h);

private:
    void initializeShader();
    void initializeTriangle();

    QOpenGLShaderProgram m_program;
    QOpenGLBuffer m_vbo;
    QMatrix4x4 m_projection;
};

#endif // TRIANGLERENDER_H
