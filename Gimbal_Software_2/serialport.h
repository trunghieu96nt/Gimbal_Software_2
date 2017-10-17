#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QQueue>
#include <QSerialPort>
#include <QMutex>
#include <QTimer>

typedef enum
{
    SP_STATUS_ACK_OK = 0,
    SP_STATUS_ACK_ERR,
    SP_STATUS_RESPOND_ERR,
    SP_STATUS_RESPOND_OK,
    SP_STATUS_NO_CONNECT,
    SP_STATUS_TIMEOUT_RD,
    SP_STATUS_TIMEOUT_WR,
} ENUM_SP_STATUS_T;

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

    QString port_Name() {return sp_Port_Name;}
    bool connect_Port(const QString &port_Name, qint32 baudrate);
    void disconnect_Port();

    void transaction(const QByteArray &request);
    void transaction_in_queue();
    ENUM_SP_STATUS_T send_Cmd_Blocking(char msgID, const QByteArray &payload, int wait_Timeout);
    void send_Cmd_Non_Blocking(char msgID, const QByteArray &payload);

signals:
    void done(ENUM_SP_STATUS_T status, const QByteArray &request, const QByteArray &response);

private slots:
    void timer_Send_Timeout();
    void serial_readyRead();

private:
    QQueue<QByteArray> request_Queue;
    QSerialPort serial;
    QString sp_Port_Name;
    qint32 sp_Baudrate;
    QMutex mutex_Send;
    QTimer timer_Send;
    QByteArray current_Request, data_Serial_Port;
};

#endif // SERIALPORT_H
