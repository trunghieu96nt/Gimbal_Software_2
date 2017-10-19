#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QStringList>
#include <QToolTip>
#include <QString>
#include <QDesktopWidget>
#include <QSignalMapper>
#include <QQueue>
#include <QMutex>
#include <QMenu>

#include "serialport.h"

typedef struct
{
    QString cmd_name;
    char cmd_msgID;
    QByteArray cmd_payload;
} STRU_CMD_T;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /* Tab Title */
    void on_btnControl_clicked();
    void on_btnSettings_clicked();

    /* Serial COM Port */
    void timer_Get_All_Params_timeout();
    void on_btnConnect_clicked();
    void timerSerialPort_timeout();
    void serial_port_done(ENUM_SP_STATUS_T status, const QByteArray &request, const QByteArray &response);

    /* Mode Button */
    void btnMode_clicked(const QString &cmd_Data);

    /* PID WR Button*/
    void btnWritePID_clicked(const QString &name);

    /* PID LineEdit */
    void leditPID_editingFinished(const QString &pid_Name);

    /* Pos & Vel button */
    void on_btnAZSetPos_clicked();
    void on_btnAZSetVel_clicked();
    void on_btnAZSetBoth_clicked();
    void on_btnAZGetPos_clicked();
    void on_btnELSetPos_clicked();
    void on_btnELSetVel_clicked();
    void on_btnELSetBoth_clicked();
    void on_btnELGetPos_clicked();

    /* Active Axis button */
    void on_btnAZActive_clicked(bool checked);
    void on_btnELActive_clicked(bool checked);

    /* Menu PlainTextEdit */
    void show_ptxtStatus_Menu(QPoint pt);
    void ptxtStatus_Clear();

    /* test */
    void timer_50ms_timeout();

private:
    Ui::MainWindow *ui;

    SerialPort serial_Port;
    QTimer *timerSerialPort;
    QStringList list_Serial_Port;

    double width_Factor, height_Factor;

    QSignalMapper *mode_Mapper = new QSignalMapper(this);
    QString setted_Mode;

    QString setted_PID_Value[2][5][5];
    QMutex mutex_PID_WR;

    bool setted_Active_Value[2] = {false, false};

    QTimer timer_Get_All_Params;
    int cmd_Counter;

    QTimer timer_50ms;

    STRU_CMD_T stru_GB_CMD[11] =
    {
        { "GET_MODE",         0x05, QByteArray::fromRawData("\x03", 1) },
        { "GET_AZ_MANUAL",    0x0F, QByteArray::fromRawData("\x01\x01", 2) },
        { "GET_AZ_POINTING",  0x0F, QByteArray::fromRawData("\x01\x02", 2) },
        { "GET_AZ_VELOCITY",  0x0F, QByteArray::fromRawData("\x01\x04", 2) },
        { "GET_AZ_CURRENT",   0x0F, QByteArray::fromRawData("\x01\x05", 2) },
        { "GET_EL_MANUAL",    0x0F, QByteArray::fromRawData("\x02\x01", 2) },
        { "GET_EL_POINTING",  0x0F, QByteArray::fromRawData("\x02\x02", 2) },
        { "GET_EL_VELOCITY",  0x0F, QByteArray::fromRawData("\x02\x04", 2) },
        { "GET_EL_CURRENT",   0x0F, QByteArray::fromRawData("\x02\x05", 2) },
        { "GET_ACTIVE_AZ",    0x11, QByteArray::fromRawData("\x01", 1) },
        { "GET_ACTIVE_AZ",    0x11, QByteArray::fromRawData("\x02", 1) },
    };

    void init_GUI();
    void init_Page();
    void init_Serial_Port();
    void init_Mode_Button_Mapping();
    void init_PID_LineEdit_Mapping();
    void init_PID_WR_Button_Mapping();

    bool get_All_Params();
    void status_Append_Text(const QString &text);
    void status_Append_Text(const QString &text, QColor color);
};

#endif // MAINWINDOW_H
