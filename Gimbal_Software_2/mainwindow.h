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
#include <QFont>

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
    void on_timerSerialPort_timeout();
    void on_serialPort_readyRead();

    void on_btnHome_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort = new QSerialPort;
    QTimer *timerSerialPort;

    QStringList list_Serial_Port;
    QByteArray data_Serial_Port;
    double width_Factor, height_Factor;

    void init_GUI();
    void init_Serial_COM_Port();

    bool serialCOMPort_write(const QByteArray &data);
    bool send_Command(char msgID, const QByteArray &payload);
    bool parse_Msg(const QByteArray &msg);
};

#endif // MAINWINDOW_H
