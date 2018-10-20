#include "cuberender.h"
#include "openglItem.h"
#include <QMouseEvent>
#include <QQuickWindow>
#include <QOpenGLTexture>

OpenGLItem::OpenGLItem()
    : m_cubeRender(0),
      m_angularSpeed(0),
      m_fps(0),
      m_lastTime(QTime::currentTime())
{
    m_timer.start(12, this);
    connect(this, &QQuickItem::windowChanged, this, [this](QQuickWindow *window)
    {
        if (window)
        {
            connect(window, &QQuickWindow::beforeSynchronizing, this, &OpenGLItem::sync,
                    Qt::DirectConnection);
            connect(window, &QQuickWindow::sceneGraphInvalidated, this, &OpenGLItem::cleanup,
                    Qt::DirectConnection);

            window->setClearBeforeRendering(false);
        }
        else return;
    });
}

OpenGLItem::~OpenGLItem()
{

}

void OpenGLItem::sync()
{
    if (!m_cubeRender)
    {
        m_cubeRender = new CubeRender();
        m_cubeRender->initializeGL();
        m_cubeRender->resizeGL(window()->width(), window()->height());    
        connect(window(), &QQuickWindow::beforeRendering, this, [this]()
        {
            //window()->resetOpenGLState();
            m_cubeRender->render(m_rotation);
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, [this]()
        {
            //渲染后调用，计算fps
            static int frame = 0;
            frame++;
            if (m_lastTime.msecsTo(QTime::currentTime()) > 1000)
            {
                setFps(frame);
                m_lastTime = QTime::currentTime();
                frame = 0;
            }
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::widthChanged, this, [this]()
        {
            m_cubeRender->resizeGL(window()->width(), window()->height());
        });
        connect(window(), &QQuickWindow::heightChanged, this, [this]()
        {
            m_cubeRender->resizeGL(window()->width(), window()->height());
        });
    }
}

void OpenGLItem::cleanup()
{
    if (m_cubeRender)
    {
        delete m_cubeRender;
        m_cubeRender = nullptr;
    }
}

void OpenGLItem::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    setAngularSpeed(m_angularSpeed * 0.99);

    if (angularSpeed() <= 0.01)
        setAngularSpeed(0.0);
    else
    {
        m_rotation = QQuaternion::fromAxisAndAngle(m_rotateAixs, m_angularSpeed) * m_rotation;
        window()->update();
    }
}

void OpenGLItem::mousePress(int x, int y)
{
    m_mouseStartPos = QVector2D(x, y);
}

void OpenGLItem::mouseRelase(int x, int y)
{
    QVector2D diff = QVector2D(x, y) - m_mouseStartPos;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    qreal acc = diff.length() / 100.0;
    setRotateAixs((rotateAixs() * angularSpeed() + n * acc).normalized());
    setAngularSpeed(angularSpeed() + acc);
}
