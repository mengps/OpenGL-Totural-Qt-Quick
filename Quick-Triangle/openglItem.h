#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <QTime>
#include <QQuickItem>
#include <QBasicTimer>

class TriangleRender;
class OpenGLItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)

public:
    OpenGLItem();
    ~OpenGLItem();

    int fps() const { return m_fps; }
    void setFps(int fps) { if (fps == m_fps) return; m_fps = fps; emit fpsChanged(); }

public slots:
    void sync();
    void cleanup();

signals:
    void fpsChanged();

protected:
    void timerEvent(QTimerEvent *event);

private:
    int m_fps;
    QTime m_lastTime;
    QBasicTimer m_timer;
    TriangleRender *m_triangleRender;
};

#endif // OPENGLWINDOW_H
