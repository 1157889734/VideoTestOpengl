#ifndef _PLANE_TEST_H
#define _PLANE_TEST_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#include <xf86drm.h>

#include "mutex.h"
#include "dev.h"
#include "bo.h"
#include "modeset.h"

#include "rgaapi.h"

typedef struct _DRM_INFO
{
    int x,y,w,h;
    int iShow;
    struct sp_dev   *dev;
    struct sp_crtc  *use_crtc;
    struct sp_plane *plane;
    RGA_HANDLE rgaHandle;
    CMutexLock locker;

}DRM_INFO, *PDRM_INFO;

PDRM_INFO DRM_Init(int x, int y, int w, int h);
int DRM_UnInit(PDRM_INFO drm_info);
int DRM_SetRect(int x, int y, int w, int h);
int DRM_Display(MppFrame frame);
int DRM_Show(int iShow);

#ifdef __cplusplus
}
#endif

#endif
