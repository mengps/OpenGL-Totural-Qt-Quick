#include "trianglerender.h"
#include "openglItem.h"
#include <QQuickWindow>

OpenGLItem::OpenGLItem()
    : m_triangleRender(0),
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
    if (!m_triangleRender)
    {
        m_triangleRender = new TriangleRender();
        m_triangleRender->initializeGL();
        m_triangleRender->resizeGL(window()->width(), window()->height());
        connect(window(), &QQuickWindow::beforeRendering, this, [this]()
        {
            //window()->resetOpenGLState();
            m_triangleRender->render();
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
            m_triangleRender->resizeGL(window()->width(), window()->height());
        });
        connect(window(), &QQuickWindow::heightChanged, this, [this]()
        {
            m_triangleRender->resizeGL(window()->width(), window()->height());
        });
    }
}

void OpenGLItem::cleanup()
{
    if (m_triangleRender)
    {
        delete m_triangleRender;
        m_triangleRender = nullptr;
    }
}

void OpenGLItem::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    window()->update();
}
