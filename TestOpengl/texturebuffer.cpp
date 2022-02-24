#include "texturebuffer.h"

#include <QElapsedTimer>
#include <QOpenGLFunctions>

#include <unistd.h>

TextureBuffer::TextureBuffer()
    : m_ready(false)
    , m_texture(0)
{
    iNv12 = 0;
    m_videoW = 0;
    m_videoH = 0;
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::createTexture(QOpenGLContext *context)
{
    context->functions()->glGenTextures(1, &m_texture);
    m_ready = true;
}

void TextureBuffer::deleteTexture(QOpenGLContext *context)
{
    context->functions()->glDeleteTextures(1, &m_texture);
}

// called in render thread
void TextureBuffer::updateTexture(QOpenGLContext *context, uchar *ptrY,uchar *ptrU,uchar *ptrV,uint width,uint height)
{
    QMutexLocker lock(&m_mutex);
    iNv12 = 0;

    auto f = context->functions();
    m_videoW = width;
    m_videoH = height;

    f->glActiveTexture(GL_TEXTURE0);  //激活纹理单元GL_TEXTURE0,系统里面的
    f->glBindTexture(GL_TEXTURE_2D,idY); //绑定y分量纹理对象id到激活的纹理单元
    //使用内存中的数据创建真正的y分量纹理数据
    f->glTexImage2D(GL_TEXTURE_2D,0,GL_RED, width, height, 0, GL_RED,GL_UNSIGNED_BYTE, ptrY);
    //f->glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 0, 0, width, height, 0);


    f->glActiveTexture(GL_TEXTURE1); //激活纹理单元GL_TEXTURE1
    f->glBindTexture(GL_TEXTURE_2D, idU);
    //使用内存中的数据创建真正的u分量纹理数据
    f->glTexImage2D(GL_TEXTURE_2D,0,GL_RED, width >> 1, height >> 1,0,GL_RED,GL_UNSIGNED_BYTE, ptrU);
    //f->glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 0, 0, width >> 1, height >> 1, 0);


    glActiveTexture(GL_TEXTURE2); //激活纹理单元GL_TEXTURE2
    glBindTexture(GL_TEXTURE_2D,idV);
    //使用内存中的数据创建真正的v分量纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width >> 1, height >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, ptrV);
    //f->glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 0, 0, width >> 1, height >> 1, 0);

    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glFinish();
}

void TextureBuffer::updateTextureRGB(uchar *ptrData, uint width,uint height)
{
    QMutexLocker lock(&m_mutex);
    iNv12 = 3;
    if(m_videoW != width || m_videoH != height)
    {
        m_videoW = width;
        m_videoH = height;
        //img = QImage(width, height, QImage::Format_RGBA8888);
        printf("sssss \n");
    }
    img = QImage((const uchar*)ptrData, width, height, QImage::Format_RGBA8888);
    //memcpy(img.bits(), ptrData, width*height*4);

    lock.unlock();
    return;
    //img.loadFromData(ptrData, width*height*4, "png");//, QImage::Format_RGBA8888);
    if(access("/home/data/1.png", F_OK))
    {
        bool br = img.save("/home/data/1.png", "png");
        printf("save png: %d \n", br);
    }
}

void TextureBuffer::updateTextureNv12(QOpenGLContext *context, uchar *ptrY,uchar *ptrUV,uint width,uint height)
{
    //QMutexLocker lock(&m_mutex);
    iNv12 = 1;
    auto f = context->functions();
    m_videoW = width;
    m_videoH = height;


    f->glActiveTexture(GL_TEXTURE0);  //激活纹理单元GL_TEXTURE0,系统里面的
    f->glBindTexture(GL_TEXTURE_2D,nv12_idY); //绑定y分量纹理对象id到激活的纹理单元
    //使用内存中的数据创建真正的y分量纹理数据
    //f->glTexImage2D(GL_TEXTURE_2D,0,GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, ptrY);
    //f->glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 0, 0, width, height, 0);


    /*
    f->glActiveTexture(GL_TEXTURE1); //激活纹理单元GL_TEXTURE1
    f->glBindTexture(GL_TEXTURE_2D, nv12_idUV);
    //使用内存中的数据创建真正的u分量纹理数据
    f->glTexImage2D(GL_TEXTURE_2D,0,GL_RG, width >> 1, height >> 1,0,GL_RG,GL_UNSIGNED_BYTE, ptrUV);
    //f->glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RG, 0, 0, width >> 1, height >> 1, 0);
*/


    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glFinish();
}

// called in main thread
void TextureBuffer::drawTexture(QOpenGLContext *context, int vertextCount)
{
    QMutexLocker lock(&m_mutex);

    auto f = context->functions();

    f->glBindTexture(GL_TEXTURE_2D, m_texture);

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    f->glActiveTexture(GL_TEXTURE0);

    f->glDrawArrays(GL_TRIANGLES, 0, vertextCount);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    //f->glFinish();
}
