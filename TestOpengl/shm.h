#ifndef SHM_H
#define SHM_H

#include <QWidget>

#include "rgaapi.h"

typedef void* SHM_HANDLE;
int SHM_Init();
int SHM_Uinit();
SHM_HANDLE SHM_AddRect(QWidget *);

int SHM_AttchWnd(SHM_HANDLE, QWidget *);
int SHM_DetchWnd(SHM_HANDLE);
int SHM_FreeRect(SHM_HANDLE hShmHandle);


int SHM_Display(SHM_HANDLE hPlaneHandle, MppFrame frame);


#endif
