#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include "render.h"
#include <QTime>
#include <QQuickItem>
#include <QBasicTimer>

class MyRender : public Render
{
public:
    MyRender() { }
    ~MyRender() { }

    void render()
    {
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class OpenGLItem : public QQuickItem
{
    Q_OBJECT

public:
    OpenGLItem();
    ~OpenGLItem();

public slots:
    void sync();
    void cleanup();

protected:
    void timerEvent(QTimerEvent *event);

private:
    QBasicTimer m_timer;
    Render *m_render;
};

#endif // OPENGLWINDOW_H
