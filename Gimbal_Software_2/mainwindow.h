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

#include "serialport.h"

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
    void serial_port_done(ENUM_SP_STATUS_T status, const QByteArray &request, const QByteArray &respond);

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

    SerialPort serial_Port;
    QTimer *timerSerialPort;
    QStringList list_Serial_Port;

    double width_Factor, height_Factor;

    QSignalMapper *mode_Mapper = new QSignalMapper(this);
    QString setted_Mode;

    QString setted_PID_Value[2][5][5];
    bool changed_PID_LineEdit[2][5][5];

    bool setted_Active_Value[2] = {false, false};

    void init_GUI();
    void init_Page();
    void init_Serial_Port();
    void init_Mode_Button_Mapping();
    void init_PID_LineEdit_Mapping();

    bool load_All_Params();
    void status_Append_Text(const QString &text);
    void status_Append_Text(const QString &text, QColor color);
};

#endif // MAINWINDOW_H
