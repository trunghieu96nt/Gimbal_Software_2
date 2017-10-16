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

#include "serialportthread.h"

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
    void on_btnConnect_clicked();
    void timerSerialPort_timeout();

    /* Mode Button */
    void btnMode_clicked(const QString &cmd_Data);

    /* PID LineEdit */
    void leditPID_editingFinished(const QString &pid_Name);
    void on_btnWritePositionLoop_clicked();
    void on_btnWriteVelocityLoop_clicked();
    void on_btnWriteCurrentLoop_clicked();

    void on_btnAZSetPos_clicked();
    void on_btnAZSetVel_clicked();
    void on_btnAZSetBoth_clicked();
    void on_btnAZGetPos_clicked();
    void on_btnELSetPos_clicked();
    void on_btnELSetVel_clicked();
    void on_btnELSetBoth_clicked();
    void on_btnELGetPos_clicked();

    void on_btnAZActive_clicked(bool checked);
    void on_btnELActive_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    SerialPortThread sport_Thread;
    QSerialPort *serialPort = new QSerialPort;
    QTimer *timerSerialPort;
    QStringList list_Serial_Port;

    double width_Factor, height_Factor;

    QSignalMapper *mode_Mapper = new QSignalMapper(this);

    QString setted_PID_Value[2][5][5];
    bool changed_PID_LineEdit[2][5][5];

    void init_GUI();
    void init_Page();
    void init_Serial_Port();
    void init_Mode_Button_Mapping();
    void init_PID_LineEdit_Mapping();

    bool load_All_Params();
    bool serialPort_write(const QByteArray &data);
    bool send_Command(char msgID, const QByteArray &payload);
    ENUM_SP_STATUS_T send_Command(char msgID, const QByteArray &payload,int wait_Timeout);
    bool parse_Msg(const QByteArray &msg);
    void status_Append_Text(const QString &text);
};

#endif // MAINWINDOW_H
