#include "renderthread.h"

#include <QDebug>
#include <QOpenGLContext>
#include <memory>

RenderThread::RenderThread(QSurface *surface, QOpenGLContext *mainContext, QObject *parent)
    : QThread(parent)
    , m_mainContext(mainContext)
    , m_surface(surface)
{
    m_running = false;
    ptVideoInfo = NULL;
    m_pTextureBuffer = NULL;
    m_renderContext = NULL;

    m_renderContext = new QOpenGLContext;
    m_renderContext->setFormat(m_mainContext->format());
    m_renderContext->setShareContext(m_mainContext);
    m_renderContext->create();
    m_renderContext->moveToThread(this);
}

RenderThread::~RenderThread()
{
    m_renderContext->destroyed();
    delete m_renderContext;
}

// called in UI thread
void RenderThread::setNewSize(int width, int height)
{
    QMutexLocker lock(&m_mutex);
    m_width = width;
    m_height = height;
}

void RenderThread::run()
{


}
