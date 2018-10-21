#include "openglItem.h"
#include <QQuickWindow>

OpenGLItem::OpenGLItem()
    : m_render(0)
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
    if (!m_render)
    {
        m_render = new MyRender();
        m_render->initializeGL();
        m_render->resizeGL(window()->width(), window()->height());
        connect(window(), &QQuickWindow::beforeRendering, this, [this]()
        {
            //window()->resetOpenGLState();
            m_render->render();
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, [this]()
        {
            //渲染后调用，计算fps
        }, Qt::DirectConnection);
        connect(window(), &QQuickWindow::widthChanged, this, [this]()
        {
            m_render->resizeGL(window()->width(), window()->height());
        });
        connect(window(), &QQuickWindow::heightChanged, this, [this]()
        {
            m_render->resizeGL(window()->width(), window()->height());
        });
    }
}

void OpenGLItem::cleanup()
{
    if (m_render)
    {
        delete m_render;
        m_render = nullptr;
    }
}

void OpenGLItem::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    window()->update();
}
