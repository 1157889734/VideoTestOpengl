#ifndef GLYUVWIDGET_H
#define GLYUVWIDGET_H



#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QPainter>
#include "renderthread.h"
#include "texturebuffer.h"
#include <QImage>
#include <QPixmap>

QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLYuvWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLYuvWidget(QWidget *parent =0);
    ~GLYuvWidget();

    void initRenderThread();

    int mw;
    int mh;
    QImage m_image;
    RenderThread  *m_thread;
    TextureBuffer *m_pTextureBuffer;

    int iNv12;
protected:
    virtual void resizeEvent(QResizeEvent*);
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShaderProgram *program;
    QOpenGLShaderProgram *programNV12;
    GLuint textureUniformY,textureUniformU,textureUniformV;
    QOpenGLTexture *textureY,*textureU,*textureV;
    GLuint idY,idU,idV;

    QOpenGLTexture *nv12_textureY,*nv12_textureUV;
    GLuint nv12_idY,nv12_idUV;
    GLuint nv12_textureUniformY,nv12_textureUniformUV;

    uint m_videoW, m_videoH;
    uchar *m_ptrY;
    uchar *m_ptrU;
    uchar *m_ptrV;

    //QPainter painter;
};


#endif // GLYUVWIDGET_H
