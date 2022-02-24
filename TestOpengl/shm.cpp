#include "shm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <linux/input-event-codes.h>

#include <QGuiApplication>
#ifdef IMX8
#include "5.10.1/QtGui/qpa/qplatformnativeinterface.h"
#else
#include "5.14.2/QtGui/qpa/qplatformnativeinterface.h"
#endif

typedef struct  T_SHM_RECT_INFO
{
    int    w;
    int    h;
    int   size;
    uchar *addr;
    RGA_HANDLE rgaHandle;
    struct wl_surface *window_handle;
    struct wl_buffer* buffer;
    CMutexLock lock;
    T_SHM_RECT_INFO()
    {
        w = h = 0;
        addr = NULL;
        rgaHandle = NULL;
        buffer = NULL;
        window_handle = NULL;
    }

}*PT_SHM_RECT_INFO;



static QPlatformNativeInterface *native = NULL;
static struct wl_display *display_handle = NULL;

static struct wl_shm        *s_shm = NULL;

static PT_SHM_RECT_INFO create_rect_info(int w, int h)
{
    int stride = w;
    int size = stride * h * 2;
    static int tmp_file_index = 0;
    char filename[64] = {0};// "/home/data/hello-wayland-XXXXXX";
    sprintf(filename, "/tmp/hello-%d-wayland-XXXXXX", tmp_file_index++);

    if(s_shm == NULL)
    {
        return NULL;
    }


    int fd = mkstemp(filename);
    if(fd < 0)
    {
        printf("mkstemp failed : %m\n");
        return NULL;
    }

    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
    ftruncate(fd, size);

    uchar *shm_data = (uchar*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_data == MAP_FAILED) {
        printf("mmap failed : %m\n");
        close(fd);
        return NULL;
    }

    unlink(filename);

    struct wl_shm_pool *pool = wl_shm_create_pool(s_shm, fd, size);
    struct wl_buffer   *buffer = wl_shm_pool_create_buffer(pool, 0, w, h, stride, WL_SHM_FORMAT_NV12);
    wl_shm_pool_destroy(pool);

    //memset(shm_data, 0x0, size);

    PT_SHM_RECT_INFO pRectInfo = new T_SHM_RECT_INFO;
    pRectInfo->addr = shm_data;
    pRectInfo->buffer = buffer;
    pRectInfo->w = w;
    pRectInfo->h = h;

    return pRectInfo;
}


static void handle_global(void *data, struct wl_registry *registry,
        uint32_t name, const char *interface, uint32_t version) {
    printf("handle_global : %s \n", interface);
    if (strcmp(interface, wl_shm_interface.name) == 0) {
        s_shm = (struct wl_shm *)wl_registry_bind(registry, name, &wl_shm_interface, 1);
    }
}

static void handle_global_remove(void *data, struct wl_registry *registry,
        uint32_t name) {
    // Who cares
}

static const struct wl_registry_listener registry_listener = {
    .global = handle_global,
    .global_remove = handle_global_remove,
};


int SHM_Init()
{
    printf("SHM_Init 0 \n");
    if(native == NULL)
        native = QGuiApplication::platformNativeInterface();
    if(display_handle == NULL)
        display_handle = (struct wl_display *)native->nativeResourceForWindow("display", NULL);

    printf("SHM_Init begin \n");
    struct wl_registry *registry = wl_display_get_registry(display_handle);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display_handle);

    printf("SHM_Init ok \n");
    return 0;
}
int SHM_Uinit()
{
    return 0;
}

int SHM_FreeRect(SHM_HANDLE hShmHandle)
{
    PT_SHM_RECT_INFO pShmRectInfo = (PT_SHM_RECT_INFO)hShmHandle;
    if(pShmRectInfo == NULL)
    {
        return -1;
    }
    pShmRectInfo->lock.Lock();
    if(pShmRectInfo->rgaHandle)
    {
        rga_destroy(pShmRectInfo->rgaHandle);
        pShmRectInfo->rgaHandle = NULL;
    }
    munmap(pShmRectInfo->addr, pShmRectInfo->size);

    pShmRectInfo->lock.Unlock();
    delete pShmRectInfo;
    pShmRectInfo = NULL;

    return 0;
}

SHM_HANDLE SHM_AddRect(QWidget *pWnd)
{
    pWnd->winId();
    struct wl_surface *window_handle  = NULL;
    window_handle = (struct wl_surface *)native->nativeResourceForWindow("surface",
                                  pWnd->windowHandle());
    if(window_handle == NULL)
    {
        printf("window_handle NULL \n");
        return NULL;
    }
    int w = pWnd->width();
    int h = pWnd->height();

    w = CODEC_ALIGN(w, 16);
    h = CODEC_ALIGN(h, 16);

    PT_SHM_RECT_INFO pShmRectInfo = create_rect_info(w, h);
    if (pShmRectInfo == NULL) {
        printf("create_buffer error \n");
        return NULL;
    }
    RGA_HANDLE rgaHandle = NULL;
    rgaHandle = rga_create(w, h);
    if(!rgaHandle)
    {
        printf("rga_create error \n");
    }
    pShmRectInfo->rgaHandle = rgaHandle;

    memset(pShmRectInfo->addr, 0x10, w * h);
    memset(pShmRectInfo->addr + w * h, 0x80, w * h * 0.5);

    pShmRectInfo->window_handle = window_handle;
    printf("wl_surface_attach, %0x \n", window_handle);
    wl_surface_attach(window_handle, pShmRectInfo->buffer, 0, 0);
    wl_surface_commit(window_handle);
    wl_display_flush(display_handle);

    return pShmRectInfo;
}

int SHM_AttchWnd(SHM_HANDLE hShmHandle, QWidget *pWnd)
{
    PT_SHM_RECT_INFO pShmRectInfo = (PT_SHM_RECT_INFO)hShmHandle;
    if(pShmRectInfo == NULL)
    {
        return -1;
    }
    pWnd->winId();
    if(pShmRectInfo->window_handle)
    {
        SHM_DetchWnd(hShmHandle);
//        return -1;
    }

    struct wl_surface *window_handle  = NULL;
    window_handle = (struct wl_surface *)native->nativeResourceForWindow("surface",
                                  pWnd->windowHandle());
    if(window_handle == NULL)
    {
        printf("window_handle NULL \n");
        return NULL;
    }
    pShmRectInfo->lock.Lock();
    printf("wl_surface_attach, %0x \n", window_handle);
    wl_surface_attach(window_handle, pShmRectInfo->buffer, 0, 0);
    wl_surface_commit(window_handle);
    wl_display_flush(display_handle);
    pShmRectInfo->window_handle = window_handle;
    pShmRectInfo->lock.Unlock();
    return 0;
}




int SHM_DetchWnd(SHM_HANDLE hShmHandle)
{
    PT_SHM_RECT_INFO pShmRectInfo = (PT_SHM_RECT_INFO)hShmHandle;
    if(pShmRectInfo == NULL)
    {
        return -1;
    }
    if(!pShmRectInfo->window_handle)
    {
        return -1;
    }
    pShmRectInfo->lock.Lock();

    wl_surface_attach(pShmRectInfo->window_handle, 0, 0, 0);


    wl_surface_commit(pShmRectInfo->window_handle);


    wl_display_flush(display_handle);
    pShmRectInfo->window_handle = NULL;
    pShmRectInfo->lock.Unlock();

    return 0;
}

int SHM_Display(SHM_HANDLE hPlaneHandle, MppFrame frame)
{
    PT_SHM_RECT_INFO pShmRectInfo = (PT_SHM_RECT_INFO)hPlaneHandle;
    if(pShmRectInfo == NULL)
    {
        printf("pShmRectInfo err \n");
        return -1;
    }

    int err, i;
    int width, height, h_stride, v_stride;
    MppBuffer buffer    = NULL;
    unsigned char* base = NULL;
    MppFrame  dstFrame  = NULL;
    RGA_HANDLE rgaHandle = pShmRectInfo->rgaHandle;
    err = rga_blit(rgaHandle, frame, &dstFrame);
    if(err < 0)
    {
        printf("rga_blit err \n");
        return -1;
    }
    err = mpp_frame_get_errinfo(dstFrame) | mpp_frame_get_discard(dstFrame);
    if (err)
    {
        printf("get err info %d discard %d, go back.\n",
               mpp_frame_get_errinfo(dstFrame),
               mpp_frame_get_discard(dstFrame));
        return -1;
    }
    h_stride = mpp_frame_get_hor_stride(dstFrame);
    v_stride = mpp_frame_get_ver_stride(dstFrame);
    width  =   mpp_frame_get_width(dstFrame);
    height =   mpp_frame_get_height(dstFrame);
    buffer = mpp_frame_get_buffer(dstFrame);
#if 0
    printf("w:%d, h:%d,h_stride:%d,v_stride:%d, bo->pitch:%d, bo->height:%d \n", width, height,
           h_stride, v_stride,
           pShmRectInfo->w, pShmRectInfo->h);
#endif

    pShmRectInfo->lock.Lock();
    if(!pShmRectInfo->window_handle)
    {
        printf("sss 0 \n");
        pShmRectInfo->lock.Unlock();
        return -1;
    }

    base = (RK_U8 *)mpp_buffer_get_ptr(buffer);
    uint8_t *luma;
    uint8_t *chroma;
    for (i = 0; i < height; i++)
    {
        luma = (uint8_t *)pShmRectInfo->addr + i*pShmRectInfo->w;
        memcpy(luma, base + i*h_stride, width);
    }
    for (i = 0; i < height / 2; i++)
    {
        chroma = (uint8_t *)pShmRectInfo->addr + (i+height)*pShmRectInfo->w;
        memcpy(chroma, base + (i+height)*h_stride, width);
    }
    wl_surface_attach(pShmRectInfo->window_handle, pShmRectInfo->buffer, 0, 0);
    wl_surface_damage (pShmRectInfo->window_handle, 0, 0,
        pShmRectInfo->w, pShmRectInfo->h);
    wl_surface_commit(pShmRectInfo->window_handle);
    //wl_display_flush(display_handle);
    pShmRectInfo->lock.Unlock();
    return 0;
}
