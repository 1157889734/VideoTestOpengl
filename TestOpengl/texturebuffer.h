#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H

#include <QMutex>
#include <QOpenGLContext>
#include <QImage>
#include <QPixmap>

class TextureBuffer
{
public:
    TextureBuffer();
    ~TextureBuffer();

    bool ready() const { return m_ready; }

    void createTexture(QOpenGLContext *context);
    void deleteTexture(QOpenGLContext *context);

    void drawTexture(QOpenGLContext *context, int vertextCount);
    void updateTexture(QOpenGLContext *context, uchar *ptrY,uchar *ptrU,uchar *ptrV,uint width,uint height);
    void updateTextureNv12(QOpenGLContext *context, uchar *ptrY,uchar *ptrUV,uint width,uint height);
    void updateTextureRGB(uchar *ptrData, uint width,uint height);

    QImage    img;
    volatile int iNv12;
    GLuint idY,idU,idV;
    GLuint nv12_idY,nv12_idUV;
    volatile uint m_videoW, m_videoH;
private:
    TextureBuffer(const TextureBuffer &) = delete;
    TextureBuffer &operator =(const TextureBuffer &) = delete;
    TextureBuffer(const TextureBuffer &&) = delete;
    TextureBuffer &operator =(const TextureBuffer &&) = delete;

private:
    mutable QMutex m_mutex;

    volatile bool m_ready;

    unsigned m_texture;
};

#endif // TEXTUREBUFFER_H
