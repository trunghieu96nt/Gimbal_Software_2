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

    void on_btnCameraCapture_clicked();

    /* send vel cmd to gimbal */
    void sendVelCmd(float az_vel, float el_vel);

    /* init image processor object */
    void initImageProcessor();

    void initFile();

private:
    Ui::MainWindow *ui;

    SerialPort serialPort;
    QTimer *timerSerialPort;
    QStringList listSerialPort;

    double widthFactor, heightFactor;

    QSignalMapper *modeMapper = new QSignalMapper(this);
    QString settedMode;

    QString settedPIDValue[2][5][5];
    QMutex mutexPIDWR;

    bool settedActiveValue[2] = {false, false};

    QTimer timerGetAllParams;
    int cmdCounter;

    QMap<QString, QString> pidTrackingParams;

    STRU_CMD_T struGB_CMD[11] =
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

    void initGUI();
    void initPage();
    void initSerialPort();
    void initModeButtonMapping();
    void initPIDLineEditMapping();
    void initPIDWRButtonMapping();

    bool getAllParams();
    void statusAppendText(const QString &text);
    void statusAppendText(const QString &text, QColor color);
};

#endif // MAINWINDOW_H
