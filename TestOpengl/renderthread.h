#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include "texturebuffer.h"
#include "vdec.h"


class RenderThread : public QThread
{
    Q_OBJECT

public:
    RenderThread(QSurface *surface, QOpenGLContext *mainContext, QObject *parent = nullptr);
    ~RenderThread();

    void setNewSize(int width, int height);
    TextureBuffer *m_pTextureBuffer;
    volatile PT_VIDEO_DEC_INFO ptVideoInfo;
    volatile bool m_running;
protected:
    virtual void run();
signals:
    void imageReady();

private:

    int m_width = 100;
    int m_height = 100;
    QMutex m_mutex;

    QSurface       *m_surface;
    QOpenGLContext *m_mainContext;
    QOpenGLContext *m_renderContext;
};

#endif // RENDERTHREAD_H
