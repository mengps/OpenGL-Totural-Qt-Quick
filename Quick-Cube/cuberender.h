#ifndef CUBERENDER_H
#define CUBERENDER_H
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class QOpenGLTexture;
class CubeRender : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    CubeRender(QObject *parent = nullptr);
    ~CubeRender();

    void initializeGL();

public slots:
    void render(const QQuaternion &rotation);
    void resizeGL(int w, int h);

private:
    void initializeShader();
    void initializeTexture();
    void initializeCube();

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo;

    QOpenGLShaderProgram m_program;
    QOpenGLTexture *m_texture;
    QMatrix4x4 m_projection;  //投影矩阵
};

#endif // CUBERENDER_H
