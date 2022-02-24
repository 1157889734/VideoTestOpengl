#include "render.h"

#ifdef USE_SDL
//#include "SDL.h"
typedef struct _T_SDL_RENDER_INFO
{
    SDL_Window      *pWnd;
    SDL_Renderer    *pRender;
    SDL_Texture     *pTexture;
    SDL_Rect         rect;
}T_SDL_RENDER_INFO;

static T_SDL_RENDER_INFO* SDLCreateRender(void* winid, int w, int h)
{
    T_SDL_RENDER_INFO* pSdlRender = new T_SDL_RENDER_INFO;
    //???????
    pSdlRender->pWnd = SDL_CreateWindowFrom(winid);
    if(pSdlRender->pWnd == NULL)
    {
        printf("SDL_CreateWindowFrom error,%x \n", winid);
    }

    SDL_SetWindowResizable(pSdlRender->pWnd, SDL_TRUE);

    pSdlRender->rect.x = 0;
    pSdlRender->rect.y = 0;
    pSdlRender->rect.w = w;
    pSdlRender->rect.h = h;

    //????
    pSdlRender->pRender = SDL_CreateRenderer(pSdlRender->pWnd, -1, SDL_RENDERER_ACCELERATED);
    if(pSdlRender->pRender == NULL)
    {
        printf("SDL_CreateRenderer error,%x \n", winid);
    }

    //SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT
    pSdlRender->pTexture = SDL_CreateTexture(pSdlRender->pRender, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, w, h);
    if(pSdlRender->pTexture == NULL)
    {
        printf("SDL_CreateTexture error,%x \n", pSdlRender->pRender);
    }

    return pSdlRender;
}
#endif

int RENDER_Init()
{
    static bool bInited = false;
    if(bInited)
    {
        return 0;
    }
    bInited = true;
    int iRet = -1;
#ifdef USE_SDL
    iRet = SDL_Init(SDL_INIT_VIDEO);

#if 1
    SDL_RendererInfo tRendererInfo;
    int iNum = SDL_GetNumRenderDrivers();
    for(int i = 0; i < iNum; i++)
    {
        memset(&tRendererInfo, 0, sizeof(SDL_RendererInfo));
        SDL_GetRenderDriverInfo(i, &tRendererInfo);
        printf("render name :%s \n", tRendererInfo.name);
    }
#endif
    iRet = SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengles2");
    printf("SDL init iRet:%d \n", iRet);
    //iRet = SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
#endif
    return iRet;
}
int RENDER_UnInit()
{
    return 0;
}
void* RENDER_Create(const T_WND_INFO* pWndInfo)
{
    #ifdef USE_SDL
    RENDER_Init();
    T_SDL_RENDER_INFO *pRenderInfo = NULL;
    pRenderInfo = SDLCreateRender(pWndInfo->hWnd, pWndInfo->nVideoWidth, pWndInfo->nVideoHeight);
    return pRenderInfo;
#endif
    return NULL;
}
void RENDER_Destroy(void* &hHandle)
{
#ifdef USE_SDL
    if(hHandle)
    {
        T_SDL_RENDER_INFO *pRenderInfo = (T_SDL_RENDER_INFO *)hHandle;
        SDL_DestroyRenderer(pRenderInfo->pRender);
        delete pRenderInfo;
        hHandle = NULL;
    }
#endif
}
void RENDER_RenderYUV(void* hHandle,
                      const unsigned char * pYplane,  int nYpitch,
                      const unsigned char * pUplane, int nUpitch,
                      const unsigned char * pVplane, int nVpitch)
{
#ifdef USE_SDL
    if(hHandle == NULL)
    {
        return;
    }

    T_SDL_RENDER_INFO *pRenderInfo = (T_SDL_RENDER_INFO *)hHandle;
    //?????
    int iRet = -1;
    SDL_UpdateYUVTexture(pRenderInfo->pTexture, &pRenderInfo->rect,
        pYplane, nYpitch,
        pUplane, nUpitch,
        pVplane, nVpitch);

    if (iRet < 0)
    {
        printf("SDL_UpdateYUVTexture err,%d, %x \n", iRet, pRenderInfo->pTexture);
    }
    iRet = SDL_RenderCopy(pRenderInfo->pRender, pRenderInfo->pTexture, &pRenderInfo->rect, NULL);
    if (iRet < 0)
    {

    }
    SDL_RenderPresent(pRenderInfo->pRender);
#endif
}

void RENDER_ChangeWnd(void* hHandle, const T_WND_INFO* pWndInfo)
{
#ifdef USE_SDL
    if(hHandle == NULL)
    {
        return;
    }
    T_SDL_RENDER_INFO *pRenderInfo = (T_SDL_RENDER_INFO *)hHandle;
    SDL_SetWindowSize(pRenderInfo->pWnd, pWndInfo->nWidth, pWndInfo->nHeight);
#endif
}
