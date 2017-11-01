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
    SP_STATUS_RESPONSE_ERR,
    SP_STATUS_RESPONSE_OK,
    SP_STATUS_NO_CONNECT,
    SP_STATUS_TIMEOUT_RD,
    SP_STATUS_TIMEOUT_WR,
} ENUM_SP_STATUS_T;

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

    QString portName() {return spPortName;}
    bool connectPort(const QString &portName, qint32 baudrate);
    void disconnectPort();

    void transaction(const QByteArray &request);
    void transactionInQueue();
    ENUM_SP_STATUS_T sendCmdBlocking(char msgID, const QByteArray &payload, int wait_Timeout, QByteArray &response);
    void sendCmdNonBlocking(char msgID, const QByteArray &payload);

signals:
    void done(ENUM_SP_STATUS_T status, const QByteArray &request, const QByteArray &response);

private slots:
    void timerSendTimeout();
    void serialReadyRead();

private:
    QQueue<QByteArray> requestQueue;
    QSerialPort serial;
    QString spPortName;
    qint32 spBaudrate;
    QMutex mutexSend;
    QTimer timerSend;
    QByteArray currentRequest, dataSerialPort;
};

#endif // SERIALPORT_H
