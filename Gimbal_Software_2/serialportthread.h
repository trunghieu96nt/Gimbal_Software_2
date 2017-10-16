#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QSerialPort>

typedef enum
{
    NO_ERROR = 0,
    ERROR_CODE_NO_PORT,
    ERROR_CODE_WAITING,
    ERROR_CODE_NO_ACK,
} ENUM_ERROR_CODE_T;

typedef enum
{
    SP_STATUS_ACK_OK = 0,
    SP_STATUS_ACK_NOT_OK,
    SP_STATUS_NO_CONNECT,
    SP_STATUS_TIMEOUT_RD,
    SP_STATUS_TIMEOUT_WR,
} ENUM_SP_STATUS_T;

class SerialPortThread : public QThread
{
    Q_OBJECT

public:
    SerialPortThread(QObject *parent = nullptr);

    /* debug */
    ENUM_ERROR_CODE_T transaction(int waitTimeout, const QByteArray &request);
    void run() Q_DECL_OVERRIDE;

    /* release */
    QString port_Name() {return spt_port_Name;}
    bool connect_Port(const QString &port_Name, qint32 baudrate);
    void disconnect_Port();
    ENUM_SP_STATUS_T send_Command_Blocking(char msgID, const QByteArray &payload, int wait_Timeout);



signals:
    /* debug */
    void response(const QByteArray &s);
    void error(const QString &s);
    void timeout(const QString &s);

    /* release */

private:
    /* debug */
    QByteArray request_Data;
    QByteArray response_Data;
    int wait_Timeout;
    QMutex mutex;

    /* release */
    QSerialPort serial;
    QString spt_port_Name;
    qint32 baudrate;
    QMutex mutex_Send;
};

#endif // SERIALPORTTHREAD_H
