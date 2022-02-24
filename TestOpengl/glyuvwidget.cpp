#include "glyuvwidget.h"

#include <QResizeEvent>

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOffscreenSurface>

#include <QDebug>
#define VERTEXIN 0
#define TEXTUREIN 1


const GLfloat vert_array[] = {
  -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f
};

const GLfloat texture_array[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

GLYuvWidget::GLYuvWidget(QWidget *parent):
    QOpenGLWidget(parent)
{
    m_ptrY = NULL;
    m_ptrU = NULL;
    m_ptrV = NULL;

    mw = 0;
    mh = 0;
    m_thread = NULL;
    m_pTextureBuffer = NULL;

    //QPixmap;

    idY = idU = idV = 0;

    nv12_idY = nv12_idUV = 0;

    iNv12 = 0;

    textureUniformY = textureUniformU = textureUniformV = 0;
    textureY = textureU = textureV = NULL;

    m_videoW = 600;
    m_videoH = 400;
    m_ptrY = new uchar[m_videoW * m_videoH];
    m_ptrU = new uchar[(m_videoW >> 1) * (m_videoH >> 1)];
    m_ptrV = new uchar[(m_videoW >> 1) * (m_videoH >> 1)];
    memset(m_ptrY, 0x00, m_videoW * m_videoH);
    memset(m_ptrU, 0x80, (m_videoW >> 1) * (m_videoH >> 1));
    memset(m_ptrV, 0x80, (m_videoW >> 1) * (m_videoH >> 1));

    m_pTextureBuffer = new TextureBuffer();
}

GLYuvWidget::~GLYuvWidget()
{
    makeCurrent();
    textureY->destroy();
    textureU->destroy();
    textureV->destroy();
    doneCurrent();
}

void GLYuvWidget::resizeEvent(QResizeEvent* ent)
{
    mw = ent->size().width();
    mh = ent->size().height();
}

void GLYuvWidget::initializeGL()
{
    bool bRet = false;
    initRenderThread();
    initializeOpenGLFunctions();

    const char *vsrc123 =
                "attribute mediump vec4 vertexIn; \
                 attribute mediump vec4 textureIn; \
                 varying mediump vec4 textureOut;  \
                 void main(void)           \
                 {                         \
                     gl_Position = vertexIn; \
                     textureOut = textureIn; \
                 }";

        const char *fsrc123 =
                "varying mediump vec4 textureOut;\n"
                "uniform sampler2D textureY;\n"
                "uniform sampler2D textureUV;\n"
                "void main(void)\n"
                "{\n"
                "mediump vec3 yuv; \n"
                "mediump vec3 rgb; \n"
                "yuv.x = texture2D(textureY, textureOut.st).r - 0.0625; \n"
                "yuv.y = texture2D(textureUV, textureOut.st).r - 0.5; \n"
                "yuv.z = texture2D(textureUV, textureOut.st).g - 0.5; \n"
                "rgb = mat3( 1,       1,         1, \n"
                            "0,       -0.39465,  2.03211, \n"
                            "1.13983, -0.58060,  0) * yuv; \n"
                "gl_FragColor = vec4(rgb, 1); \n"
                "}\n";

        programNV12 = new QOpenGLShaderProgram(this);
        programNV12->addShaderFromSourceCode(QOpenGLShader::Vertex,vsrc123);
        programNV12->addShaderFromSourceCode(QOpenGLShader::Fragment,fsrc123);
        programNV12->bindAttributeLocation("vertexIn",VERTEXIN);
        programNV12->bindAttributeLocation("textureIn",TEXTUREIN);

        bRet = programNV12->link();
        bRet = programNV12->bind();
        programNV12->enableAttributeArray(VERTEXIN);
        programNV12->setAttributeBuffer(VERTEXIN,GL_FLOAT,0,2,2*sizeof(GLfloat));
        programNV12->enableAttributeArray(TEXTUREIN);
        programNV12->setAttributeBuffer(TEXTUREIN,GL_FLOAT, 2*4*sizeof(GLfloat), 2, 2*sizeof(GLfloat));

        nv12_textureUniformY = programNV12->uniformLocation("textureY");
        nv12_textureUniformUV = programNV12->uniformLocation("textureUV");

        nv12_textureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
        nv12_textureUV = new QOpenGLTexture(QOpenGLTexture::Target2D);
        bRet = nv12_textureY->create();
        bRet = nv12_textureUV->create();
        nv12_idY = nv12_textureY->textureId();
        nv12_idUV = nv12_textureUV->textureId();


    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    const char *vsrc =
       "attribute highp vec4 vertexIn; \
        attribute highp vec2 textureIn; \
        varying highp vec2 textureOut;  \
        void main(void)           \
        {                         \
            gl_Position = vertexIn; \
            textureOut = textureIn; \
        }";
    bRet = vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment,this);
    const char *fsrc = "varying highp vec2 textureOut; \
        uniform sampler2D tex_y; \
        uniform sampler2D tex_u; \
        uniform sampler2D tex_v; \
        void main(void) \
        { \
            mediump vec3 yuv; \
            mediump vec3 rgb; \
            yuv.x = texture2D(tex_y, textureOut).r; \
            yuv.y = texture2D(tex_u, textureOut).r - 0.5; \
            yuv.z = texture2D(tex_v, textureOut).r - 0.5; \
            rgb = mat3( 1,       1,         1, \
                        0,       -0.39465,  2.03211, \
                        1.13983, -0.58060,  0) * yuv; \
            gl_FragColor = vec4(rgb, 1); \
        }";
    bRet = fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram(this);
    bRet = program->addShader(vshader);
    bRet = program->addShader(fshader);
    program->bindAttributeLocation("vertexIn",VERTEXIN);
    program->bindAttributeLocation("textureIn",TEXTUREIN);
    bRet = program->link();
    bRet = program->bind();
    program->enableAttributeArray(VERTEXIN);
    program->setAttributeBuffer(VERTEXIN,GL_FLOAT,0,2,2*sizeof(GLfloat));
    program->enableAttributeArray(TEXTUREIN);
    program->setAttributeBuffer(TEXTUREIN,GL_FLOAT, 8*sizeof(GLfloat), 2, 2*sizeof(GLfloat));

    textureUniformY = program->uniformLocation("tex_y");
    textureUniformU = program->uniformLocation("tex_u");
    textureUniformV = program->uniformLocation("tex_v");
    textureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    textureU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    textureV = new QOpenGLTexture(QOpenGLTexture::Target2D);
    bRet = textureY->create();
    bRet = textureU->create();
    bRet = textureV->create();
    idY = textureY->textureId();
    idU = textureU->textureId();
    idV = textureV->textureId();
    glClearColor(0.0,0.0,0.0,0.0);

    m_pTextureBuffer->idY = idY;
    m_pTextureBuffer->idU = idU;
    m_pTextureBuffer->idV = idV;

    m_pTextureBuffer->nv12_idY = nv12_idY;
    m_pTextureBuffer->nv12_idUV = nv12_idUV;


    printf(" lxy 1 \n ");
    //painter.begin(this);
    //painter.beginNativePainting();
    printf(" lxy initializeGL \n ");

    m_pTextureBuffer->updateTexture(QOpenGLContext::currentContext(), m_ptrY, m_ptrU, m_ptrV, m_videoW, m_videoH);
}
void GLYuvWidget::paintGL()
{
    bool bRet = false;
    if(!m_thread->m_running)
    {
        return;
    }
    if(m_pTextureBuffer->iNv12 == 3)
    {
        static QPainter painter;
        painter.begin(this);
        //qDebug() << this->geometry() << m_pTextureBuffer->img.size();
        painter.drawImage(this->geometry(), m_pTextureBuffer->img);
        painter.end();
    }
    return;
#if 0

    //printf("paint begin \n");
    QPainter painter;
    painter.begin(this);




    painter.beginNativePainting();

    glViewport(0, 0, width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLContext *context = QOpenGLContext::currentContext();
    auto f = context->functions();

    if(m_pTextureBuffer->iNv12)
    {
        bRet = programNV12->bind();
        f->glVertexAttribPointer(VERTEXIN, 2, GL_FLOAT, GL_FALSE, 0, vert_array);
        programNV12->enableAttributeArray(VERTEXIN);

        f->glVertexAttribPointer(TEXTUREIN, 2, GL_FLOAT, GL_FALSE, 0, texture_array);
        programNV12->enableAttributeArray(TEXTUREIN);

        f->glActiveTexture(GL_TEXTURE0);  //激活纹理单元GL_TEXTURE0,系统里面的
        f->glBindTexture(GL_TEXTURE_2D,nv12_idY); //绑定y分量纹理对象id到激活的纹理单元
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        f->glActiveTexture(GL_TEXTURE1); //激活纹理单元GL_TEXTURE1
        f->glBindTexture(GL_TEXTURE_2D,nv12_idUV);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


        //指定y纹理要使用新值
        f->glUniform1i(nv12_textureUniformY, 0);
        //指定u纹理要使用新值
        f->glUniform1i(nv12_textureUniformUV, 1);

        //使用顶点数组方式绘制图形
        f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        f->glBindTexture(GL_TEXTURE_2D, 0);

        programNV12->release();
    }
    else
    {
        bRet = program->bind();
        f->glVertexAttribPointer(VERTEXIN, 2, GL_FLOAT, GL_FALSE, 0, vert_array);
        program->enableAttributeArray(VERTEXIN);

        f->glVertexAttribPointer(TEXTUREIN, 2, GL_FLOAT, GL_FALSE, 0, texture_array);
        program->enableAttributeArray(TEXTUREIN);

        f->glActiveTexture(GL_TEXTURE0);  //激活纹理单元GL_TEXTURE0,系统里面的
        f->glBindTexture(GL_TEXTURE_2D,idY); //绑定y分量纹理对象id到激活的纹理单元
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        f->glActiveTexture(GL_TEXTURE1); //激活纹理单元GL_TEXTURE1
        f->glBindTexture(GL_TEXTURE_2D,idU);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        f->glActiveTexture(GL_TEXTURE2); //激活纹理单元GL_TEXTURE2
        f->glBindTexture(GL_TEXTURE_2D,idV);
        //使用内存中的数据创建真正的v分量纹理数据
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //指定y纹理要使用新值
        f->glUniform1i(textureUniformY, 0);
        //指定u纹理要使用新值
        f->glUniform1i(textureUniformU, 1);
        //指定v纹理要使用新值
        f->glUniform1i(textureUniformV, 2);

        //使用顶点数组方式绘制图形
        f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        f->glBindTexture(GL_TEXTURE_2D, 0);

        program->release();
    }


    painter.endNativePainting();
    painter.end();
    //printf("paint end \n");
#endif

#if 0
    glActiveTexture(GL_TEXTURE0);  //激活纹理单元GL_TEXTURE0,系统里面的
    glBindTexture(GL_TEXTURE_2D,idY); //绑定y分量纹理对象id到激活的纹理单元
    //使用内存中的数据创建真正的y分量纹理数据
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,m_videoW, m_videoH,0,GL_RED,GL_UNSIGNED_BYTE,m_ptrY);
    //https://blog.csdn.net/xipiaoyouzi/article/details/53584798 纹理参数解析
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE1); //激活纹理单元GL_TEXTURE1
    glBindTexture(GL_TEXTURE_2D,idU);
    //使用内存中的数据创建真正的u分量纹理数据
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,m_videoW >> 1, m_videoH >> 1,0,GL_RED,GL_UNSIGNED_BYTE, m_ptrU);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE2); //激活纹理单元GL_TEXTURE2
    glBindTexture(GL_TEXTURE_2D,idV);
    //使用内存中的数据创建真正的v分量纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_videoW >> 1, m_videoH >> 1, 0, GL_RED, GL_UNSIGNED_BYTE, m_ptrV);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //指定y纹理要使用新值
    glUniform1i(textureUniformY, 0);
    //指定u纹理要使用新值
    glUniform1i(textureUniformU, 1);
    //指定v纹理要使用新值
    glUniform1i(textureUniformV, 2);
    //使用顶点数组方式绘制图形
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

#endif
}

void GLYuvWidget::initRenderThread()
{

#if 1
    auto context = QOpenGLContext::currentContext();
    auto mainSurface = context->surface();
    auto renderSurface = new QOffscreenSurface(nullptr, this);
    renderSurface->setFormat(context->format());
    renderSurface->create();
    context->doneCurrent();
    m_thread = new RenderThread(renderSurface, context, this);
    context->makeCurrent(mainSurface);
    m_thread->m_pTextureBuffer = m_pTextureBuffer;

#endif

    //m_thread = new RenderThread(NULL, NULL, this);
    //m_thread->m_pTextureBuffer = m_pTextureBuffer;

    connect(m_thread, &RenderThread::imageReady, this, [this](){
        update();
    }, Qt::QueuedConnection);
}
