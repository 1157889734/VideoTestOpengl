#ifndef RENDER_H
#define RENDER_H

#ifdef _WIN32
#else
typedef void* HWND;
#endif

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
}

//#define USE_SDL

typedef int(*PLAYFUNC)(AVFrame	*pAVFrame, void *param);

typedef struct _T_WND_INFO
{
    HWND hWnd;
    int  nX;
    int  nY;
    int  nWidth;
    int  nHeight;
    int  nVideoWidth;
    int  nVideoHeight;
    void *pRenderHandle;

}T_WND_INFO;

int RENDER_Init();
int RENDER_UnInit();
void* RENDER_Create(const T_WND_INFO* pWndInfo);
void RENDER_Destroy(void* &hHandle);

void RENDER_RenderYUV(void* hHandle,
                      const unsigned char * pYplane,  int nYpitch,
                      const unsigned char * pUplane, int nUpitch,
                      const unsigned char * pVplane, int nVpitch);
void RENDER_ChangeWnd(void* hHandle, const T_WND_INFO* pWndInfo);

#endif // RENDER_H
