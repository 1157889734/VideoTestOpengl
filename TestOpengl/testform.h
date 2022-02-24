#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>
#include <QTimer>

#include "CMPlayerInterface.h"
#include "glyuvwidget.h"

#define VIDEO_WINDOWS_COUNT 4
#define VIDEO_WINDOWS_MaxCOUNT 5
namespace Ui {
class TestForm;
}


#include <QThread>


class TestForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestForm(QWidget *parent = 0);
    ~TestForm();

    void ResizeControl(int Width,int Height);

    //设置样式表
    void SetControlStyleSheet();

    void InitControl();


    virtual void resizeEvent(QResizeEvent *);

public slots:
     void time_out();
private slots:
     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

     void on_stackedWidget_currentChanged(int arg1);

     void on_pushButton_4_clicked();

private:
    Ui::TestForm *ui;
    int  MainState;
    int  SubState;
    QTimer *timer;
    int LastSelectCarriageIdx;
    int CurSelectCarriageIdx;

    int  m_iCurrentPage;
    int  iVideoFormat;
    bool bSingleVideoWin;

    T_WND_INFO                     m_RealMonitorVideos[VIDEO_WINDOWS_COUNT];
    GLYuvWidget *pW1[VIDEO_WINDOWS_COUNT];
    CMPHandle    pCMPHandle[VIDEO_WINDOWS_COUNT];
    T_CAMERA_INFO m_tCameraInfo[VIDEO_WINDOWS_MaxCOUNT];    //保存的所有摄像机信息

};

#endif // TESTFORM_H
