#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <QTime>
#include <QQuickItem>
#include <QBasicTimer>
#include <QVector3D>
#include <QQuaternion>

class CubeRender;
class OpenGLItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(qreal angularSpeed READ angularSpeed WRITE setAngularSpeed NOTIFY angularSpeedChanged)
    Q_PROPERTY(QVector3D rotateAixs READ rotateAixs WRITE setRotateAixs NOTIFY rotateAixsChanged)

public:
    OpenGLItem();
    ~OpenGLItem();

    int fps() const { return m_fps; }
    void setFps(int fps) { if (fps == m_fps) return; m_fps = fps; emit fpsChanged(); }
    qreal angularSpeed() const { return m_angularSpeed; }
    void setAngularSpeed(qreal angularSpeed)
    {
        if (angularSpeed == m_angularSpeed) return;
        m_angularSpeed = angularSpeed;
        emit angularSpeedChanged();
    }
    QVector3D rotateAixs() const { return m_rotateAixs; }
    void setRotateAixs(const QVector3D &rotateAixs)
    {
        if (rotateAixs == m_rotateAixs) return;
        m_rotateAixs = rotateAixs;
        emit rotateAixsChanged();
    }

public slots:
    void sync();
    void cleanup();

    void mousePress(int x, int y);
    void mouseRelase(int x, int y);

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void fpsChanged();
    void angularSpeedChanged();
    void rotateAixsChanged();

private:
    int m_fps;
    QTime m_lastTime;
    QBasicTimer m_timer;
    CubeRender *m_cubeRender;

    QVector2D m_mouseStartPos;
    QVector3D m_rotateAixs;
    qreal m_angularSpeed;
    QQuaternion m_rotation;
};

#endif // OPENGLWINDOW_H
