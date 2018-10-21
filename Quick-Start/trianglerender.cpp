#include "trianglerender.h"

struct VertexData
{
    QVector3D position;
    QVector3D color;
};

TriangleRender::TriangleRender()
    : m_vbo(QOpenGLBuffer::VertexBuffer)
{

}

TriangleRender::~TriangleRender()
{
    m_vbo.destroy();
}

void TriangleRender::initializeGL()
{
    initializeOpenGLFunctions();
    initializeShader();
    initializeTriangle();
}

void TriangleRender::initializeShader()
{
#ifdef Q_OS_WIN //使用桌面版opengl 3.3
    QString vertexFile = ":/GLSL/vertex_glsl.vert";
    QString fragmentFile = ":/GLSL/fragment_glsl.frag";
#else //使用opengl es
    QString vertexFile = ":/GLSL/vertex_glsl_es.vert";
    QString fragmentFile = ":/GLSL/fragment_glsl_es.frag";
#endif

#if QT_VERSION_MINOR >= 9   //Qt 5.9 版本加入addCacheableShaderFromSourceFile
    if (!m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile))
        qDebug() << m_program.log();
    if (!m_program.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile))
        qDebug() << m_program.log();
#else
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFile))
        qDebug() << m_program.log();
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFile))
        qDebug() << m_program.log();
#endif

    if (!m_program.link())
        qDebug() << m_program.log();

    if (!m_program.bind())
        qDebug() << m_program.log();
}

void TriangleRender::initializeTriangle()
{
    VertexData vertices[] =
    {
        { QVector3D(-0.5f, -0.5f, 0.0f), QVector3D(1.0, 0.0, 0.0) },
        { QVector3D(0.0f, 0.5f, 0.0f),   QVector3D(0.0, 1.0, 0.0) },
        { QVector3D(0.5f, -0.5f, 0.0f),  QVector3D(0.0, 0.0, 1.0) }
    };

    m_vbo.create();

    m_vbo.bind();
    m_vbo.allocate(vertices, 3 * sizeof (VertexData));
}

void TriangleRender::render()
{
    static float angle = 0.0;

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    angle += 1.0f;
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(0.0, 0.0, -5.0);
    modelMatrix.rotate(angle, 0.0f, 1.0f, 0.0f);

    m_program.bind();
    m_program.setUniformValue("mvp",  m_projection * modelMatrix);

    m_vbo.bind();

#ifdef Q_OS_WIN
    int location = 0;
    //position
    m_program.enableAttributeArray(location);
    m_program.setAttributeBuffer(location, GL_FLOAT, 0, 3, sizeof(VertexData));
    //color
    m_program.enableAttributeArray(location + 1);
    m_program.setAttributeBuffer(location + 1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData));
#else
    int vertexLocation = m_program.attributeLocation("position");
    m_program.enableAttributeArray(vertexLocation);
    m_program.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    int colorLocation = m_program.attributeLocation("color0");
    m_program.enableAttributeArray(colorLocation);
    m_program.setAttributeBuffer(colorLocation, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData));
#endif

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TriangleRender::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    qreal aspect = qreal(w) / qreal(h);
    m_projection.setToIdentity();
    m_projection.perspective(30.0, aspect, 3.0, 10.0);
}
