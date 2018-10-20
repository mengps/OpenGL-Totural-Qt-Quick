#include "cuberender.h"
#include <QOpenGLTexture>

struct VertexData
{
    QVector3D position;     //顶点坐标
    QVector3D color;        //输入颜色
    QVector2D texCoord;     //纹理坐标
};

CubeRender::CubeRender(QObject *parent)
    : QObject(parent),
      m_vbo(QOpenGLBuffer::VertexBuffer),
      m_ebo(QOpenGLBuffer::IndexBuffer),
      m_texture(0)
{

}

CubeRender::~CubeRender()
{
    m_vbo.destroy();
    m_ebo.destroy();

    delete m_texture;
}

void CubeRender::initializeGL()
{
    initializeOpenGLFunctions();

    initializeShader();
    initializeTexture();

    m_vbo.create();
    m_ebo.create();

    initializeCube();
}

void CubeRender::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    qreal aspect = qreal(w) / qreal(h);
    m_projection.setToIdentity();
    m_projection.perspective(60.0, aspect, 3.0, 10.0);
}

void CubeRender::initializeShader()
{
#ifdef Q_OS_WIN //使用桌面版opengl 3.3
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/GLSL/vertex_glsl.vert"))
        qDebug() << m_program.log();

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GLSL/fragment_glsl.frag"))
        qDebug() << m_program.log();
#else   //使用opengl es
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/GLSL/vertex_glsl_es.vert"))
        qDebug() << m_program.log();

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/GLSL/fragment_glsl_es.frag"))
        qDebug() << m_program.log();
#endif
    if (!m_program.link())
        qDebug() << m_program.log();

    if (!m_program.bind())
        qDebug() << m_program.log();
}

void CubeRender::initializeTexture()
{
    //使用mirrored()进行镜像翻转，因为QImage和Opengl的坐标是相反的
    m_texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void CubeRender::render(const QQuaternion &rotation)
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_texture->bind();

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    m_program.bind();
    m_program.setUniformValue("mvp", m_projection * matrix);
    m_program.setUniformValue("texture", 0);

    m_vbo.bind();
    m_ebo.bind();

#ifdef Q_OS_WIN
    int location = 0;
    //position
    m_program.enableAttributeArray(location);
    m_program.setAttributeBuffer(location, GL_FLOAT, 0, 3, sizeof(VertexData));
    //color
    m_program.enableAttributeArray(location + 1);
    m_program.setAttributeBuffer(location + 1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData));
    //texCoord
    m_program.enableAttributeArray(location + 2);
    m_program.setAttributeBuffer(location + 2, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(VertexData));
#else
    int vertexLocation = m_program.attributeLocation("position");
    m_program.enableAttributeArray(vertexLocation);
    m_program.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    int colorLocation = m_program.attributeLocation("color0");
    m_program.enableAttributeArray(colorLocation);
    m_program.setAttributeBuffer(colorLocation, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData));

    int texcoordLocation = m_program.attributeLocation("texCoord0");
    m_program.enableAttributeArray(texcoordLocation);
    m_program.setAttributeBuffer(texcoordLocation, GL_FLOAT, sizeof(QVector3D) * 2, 2, sizeof(VertexData));
#endif

    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}

void CubeRender::initializeCube()
{
    VertexData vertices[] =
    {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 1.0f, 1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(1.0f, 1.0f, 0.0f), QVector2D(0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.7f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.5f, 0.0f, 0.7f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.4f, 0.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(1.0f, 1.0f, 0.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.5f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.5f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.7f, 0.5f, 0.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.5f, 0.5f), QVector2D(0.66f, 1.0f)}  // v23
    };

    GLushort indices[] =
    {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
       12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
       16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
       20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    m_vbo.bind();
    m_vbo.allocate(vertices, 24 * sizeof (VertexData));

    m_ebo.bind();
    m_ebo.allocate(indices, 34 * sizeof (GLushort));
}
