#include "testform.h"
#include "ui_testform.h"

#include <QDebug>
#include <QDesktopWidget>

TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);//无边框

    memset(m_RealMonitorVideos, 0, sizeof(m_RealMonitorVideos));

    memset(m_tCameraInfo, 0, sizeof(T_CAMERA_INFO));
    m_tCameraInfo->cmpHandle = NULL;
    /*
    QWidget * pWnds[] = {ui->widget_MonitorWin1, ui->widget_MonitorWin2, ui->widget_MonitorWin3, ui->widget_MonitorWin4};
    for(int i = 0; i < VIDEO_WINDOWS_COUNT; i++)
    {
         pW1[i] = new GLYuvWidget(pWnds[i]);
    }*/

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time_out()));
    timer->start(2000*3);






}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::resizeEvent(QResizeEvent *)
{
//    QWidget * pWnds[] = {ui->widget_MonitorWin1, ui->widget_MonitorWin2, ui->widget_MonitorWin3, ui->widget_MonitorWin4};
//    for(int i = 0; i < VIDEO_WINDOWS_COUNT; i++)
//    {
//         //pW1[i]->resize(pWnds[i]->size());
//    }
}

/*********************************样式参数设置************************/
//设置控件样式表
void TestForm::SetControlStyleSheet()
{


}


void TestForm::ResizeControl(int Width,int Height)
{


}

 void TestForm::InitControl()
 {

 }

 void TestForm::time_out()
 {
     static int s_ii = 0;
     static int x = 0;
     const char * rtsp_url[] = {
         "rtsp://admin:12345@192.168.104.201", "rtsp://admin:12345@192.168.104.201","rtsp://admin:12345@192.168.104.201","rtsp://admin:12345@192.168.104.201"};

     if(x == 0)
     {
         x = 1;
         s_ii = 1;

         QRect rt;
         QPoint pt;
         QWidget *pWnds[] = {ui->widget_play,ui->widget_play2,ui->widget_MonitorWin3,ui->widget_MonitorWin4};
         for(int i = 0; i < VIDEO_WINDOWS_COUNT; i++)
         {
             rt = pWnds[i]->geometry();
             pt = pWnds[i]->mapToGlobal(QPoint(0, 0));
             m_RealMonitorVideos[i].nVideoWidth = 0;
             m_RealMonitorVideos[i].nVideoHeight = 0;
             m_RealMonitorVideos[i].nX = rt.x();
             m_RealMonitorVideos[i].nY = rt.y();
             m_RealMonitorVideos[i].nWidth = rt.width();
             m_RealMonitorVideos[i].nHeight = rt.height();
             m_RealMonitorVideos[i].hWnd = (HWND)pWnds[i];

             m_tCameraInfo[i].cmpHandle = CMP_Init(&m_RealMonitorVideos[i], CMP_VDEC_NORMAL);
             if(m_tCameraInfo[i].cmpHandle == NULL)
             {
               qDebug()<<"**************CMP_Init error**********"<<__LINE__<<endl;
             }

         }
           for(int i = 0; i < VIDEO_WINDOWS_COUNT; i++)
         {
           if(m_tCameraInfo[i].cmpHandle != NULL)
            {
                CMP_OpenMediaPreview(m_tCameraInfo[i].cmpHandle, rtsp_url[i], CMP_TCP);
                CMP_SetPlayEnnable(m_tCameraInfo[i].cmpHandle, 1);
            }
//            CMP_PlayMedia(m_tCameraInfo[i].cmpHandle);

         }
           qDebug()<<"***************play"<<endl;
     }
     else
     {
         x = 0;
         for(int i = 0; i < VIDEO_WINDOWS_COUNT; i++)
        {
            if(m_tCameraInfo[i].cmpHandle != NULL)
            {
                CMP_CloseMedia(m_tCameraInfo[i].cmpHandle);
                CMP_UnInit(m_tCameraInfo[i].cmpHandle);
            }
         }
         qDebug()<<"***************stop"<<endl;
     }
 }

void TestForm::on_pushButton_clicked()
{
//    QRect rt;
//    QPoint pt;
//    rt = ui->widget_play->geometry();
//    pt = ui->widget_play->mapToGlobal(QPoint(0, 0));
//    CMP_SetPlayEnnable(pCMPHandle[0], 0);
//    CMP_SetPlayEnnable(pCMPHandle[1], 0);
//    DRM_SetRect(pt.x(), pt.y(), rt.width(), rt.height());
//    DRM_Show(1);
//    CMP_PlayMedia(pCMPHandle[0]);
//    CMP_SetPlayEnnable(pCMPHandle[0], 1);
//    ui->stackedWidget->setCurrentIndex(0);

}

void TestForm::on_pushButton_2_clicked()
{
//    QRect rt;
//    QPoint pt;
//    rt = ui->widget_play2->geometry();
//    pt = ui->widget_play2->mapToGlobal(QPoint(0, 0));
//    CMP_SetPlayEnnable(pCMPHandle[0], 0);
//    CMP_SetPlayEnnable(pCMPHandle[1], 0);
//    DRM_SetRect(pt.x(), pt.y(), rt.width(), rt.height());
//    DRM_Show(1);
//    CMP_PlayMedia(pCMPHandle[1]);
//    CMP_SetPlayEnnable(pCMPHandle[1], 1);
//    ui->stackedWidget->setCurrentIndex(1);
}

void TestForm::on_stackedWidget_currentChanged(int arg1)
{

}

void TestForm::on_pushButton_4_clicked()
{
//    CMP_SetPlayEnnable(pCMPHandle[0], 0);
//    CMP_SetPlayEnnable(pCMPHandle[1], 0);
//    DRM_Show(0);
//    ui->stackedWidget->setCurrentIndex(2);
}
