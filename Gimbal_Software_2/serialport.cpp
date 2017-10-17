#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    timer_Send.setSingleShot(true);
    timer_Send.setInterval(2000);
    connect(&timer_Send, SIGNAL(timeout()), this, SLOT(timer_Send_Timeout()));

    connect(&serial, SIGNAL(readyRead()), this, SLOT(serial_readyRead()));;
}

/* open and close port */
bool SerialPort::connect_Port(const QString &port_Name, qint32 baudrate)
{
    this->sp_Port_Name = port_Name;
    this->sp_Baudrate = baudrate;

    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    serial.setPortName(this->sp_Port_Name);
    serial.setBaudRate(this->sp_Baudrate);

    return serial.open(QSerialPort::ReadWrite);
}

void SerialPort::disconnect_Port()
{
    serial.close();
}

/* send and receive */
void SerialPort::transaction(const QByteArray &request)
{
    this->request_Queue.enqueue(request);

    if (timer_Send.isActive() == false)
    {
        this->current_Request = this->request_Queue.dequeue();
        if (serial.isOpen() == false)
        {
            emit this->done(SP_STATUS_NO_CONNECT, this->current_Request);
            this->current_Request.clear();
            this->request_Queue.clear();
        }
        else
        {
            serial.write(this->current_Request);
            if (serial.waitForBytesWritten(10))
            {
                timer_Send.start();
            }
            else
            {
                emit this->done(SP_STATUS_TIMEOUT_WR, this->current_Request);
                this->current_Request.clear();
                this->request_Queue.clear();
            }
        }
    }
}

void SerialPort::transaction_in_queue()
{
    if (this->request_Queue.isEmpty() == false)
    {
        this->current_Request = this->request_Queue.dequeue();
        if (serial.isOpen() == false)
        {
            emit this->done(SP_STATUS_NO_CONNECT, this->current_Request);
            this->current_Request.clear();
            this->request_Queue.clear();
        }
        else
        {
            serial.write(this->current_Request);
            if (serial.waitForBytesWritten(10))
            {
                timer_Send.start();
            }
            else
            {
                emit this->done(SP_STATUS_TIMEOUT_WR, this->current_Request);
                this->current_Request.clear();
                this->request_Queue.clear();
            }
        }
    }
}

void SerialPort::timer_Send_Timeout()
{
    emit this->done(SP_STATUS_TIMEOUT_RD, this->current_Request);
    this->current_Request.clear();
    this->transaction_in_queue();
}

void SerialPort::serial_readyRead()
{
    int idx_GB, msg_Length;
    QByteArray respond;

    this->data_Serial_Port.append(serial.readAll());
    idx_GB = data_Serial_Port.indexOf("GB");
    if (idx_GB == -1) return;

    /* remove invalid data */
    data_Serial_Port.remove(0, idx_GB);

    /* wait size until greater than 6 */
    if (data_Serial_Port.size() < 6) return;
    msg_Length = data_Serial_Port.at(5) + 6;

    /* wait size until greater than msg_Length */
    if (data_Serial_Port.size() >= msg_Length) return;

    /* get respond */
    respond = data_Serial_Port.left(msg_Length);
    data_Serial_Port.remove(0, msg_Length);

    /* check respond */
    if (this->current_Request.isNull() == true) return;

    timer_Send.stop();
    if (respond.size() < 10)
    {
        emit this->done(SP_STATUS_RESPOND_ERR, this->current_Request);
        this->current_Request.clear();
    }
    else
    {
        quint16 checksum = 0, checksum_Mask = 0;
        checksum_Mask = (respond.at(respond.size() - 2) << 8) & 0x0ff00;
        checksum_Mask += respond.at(respond.size() - 1) & 0x0ff;
        for (int i = 0; i < (respond.size() - 2); i++)
        {
            checksum += static_cast<unsigned char>(respond.at(i));
        }
        checksum = ~checksum;

        if ((checksum_Mask != checksum) || (respond.at(2) != 0x01) || (respond.at(6) != this->current_Request.at(6)))
        {
            emit this->done(SP_STATUS_RESPOND_ERR, this->current_Request);
            this->current_Request.clear();
        }
        else
        {
            emit this->done(SP_STATUS_RESPOND_OK, respond);
            this->current_Request.clear();
        }
    }
    this->transaction_in_queue();
}

/* Packaged Send */
ENUM_SP_STATUS_T SerialPort::send_Cmd_Blocking(char msgID, const QByteArray &payload, int wait_Timeout)
{
    QByteArray request;
    QByteArray response;
    quint16 checkSum = 0;
    ENUM_SP_STATUS_T status = SP_STATUS_RESPOND_OK;

    request.clear();
    response.clear();

    /* Header */
    request.append(QByteArray::fromRawData("\x47\x42\x02\x01\x00", 5));

    /* Length */
    request.append((char)(1 + payload.count() + 2));

    /* MsgID */
    request.append(msgID);

    /* Payload */
    request.append(payload);

    /* Check Sum */
    for (int i = 0; i < request.count(); i++)
    {
        checkSum += static_cast<unsigned char>(request.at(i));
    }
    checkSum = ~checkSum;
    request.append((char)((checkSum >> 8) & 0x0ff));
    request.append((char)(checkSum & 0x0ff));


    /* Sending */
    if (serial.isOpen() == false)
    {
        status = SP_STATUS_NO_CONNECT;
    }
    else
    {
        mutex_Send.lock();
        serial.write(request);
        if (serial.waitForBytesWritten(wait_Timeout))
        {
            /* read response */
            if (serial.waitForReadyRead(wait_Timeout))
            {
                response = serial.readAll();
                while (serial.waitForReadyRead(10))
                    response += serial.readAll();
            }
            else
            {
                status = SP_STATUS_TIMEOUT_RD;
            }
        } else
        {
            status = SP_STATUS_TIMEOUT_WR;
        }
        mutex_Send.unlock();
    }

    /* check ack */
    if (response.isNull() == false)
    {
        quint16 checksum = 0, checksum_Mask = 0;

        /* Check DstID */
        if (response.size() < 3) return SP_STATUS_RESPOND_ERR;
        if (response.at(2) != 0x01) return SP_STATUS_RESPOND_ERR;

        /* Checksum */
        checksum_Mask = (response.at(response.size() - 2) << 8) & 0x0ff00;
        checksum_Mask += response.at(response.size() - 1) & 0x0ff;
        for (int i = 0; i < (response.size() - 2); i++)
        {
            checksum += static_cast<unsigned char>(response.at(i));
        }
        checksum = ~checksum;
        if (checksum_Mask != checksum) return SP_STATUS_RESPOND_ERR;

        /* Handle Respond Set Message */
        if (request.at(6) != response.at(6)) return SP_STATUS_RESPOND_ERR;
        if (request.at(7) != response.at(7)) return SP_STATUS_RESPOND_ERR;
    }
    return status;
}

void SerialPort::send_Cmd_Non_Blocking(char msgID, const QByteArray &payload)
{
    QByteArray request;
    quint16 checkSum = 0;

    /* Header */
    request.append(QByteArray::fromRawData("\x47\x42\x02\x01\x00", 5));

    /* Length */
    request.append((char)(1 + payload.count() + 2));

    /* MsgID */
    request.append(msgID);

    /* Payload */
    request.append(payload);

    /* Check Sum */
    for (int i = 0; i < request.count(); i++)
    {
        checkSum += static_cast<unsigned char>(request.at(i));
    }
    checkSum = ~checkSum;
    request.append((char)((checkSum >> 8) & 0x0ff));
    request.append((char)(checkSum & 0x0ff));

    /* Sending */
    this->transaction(request);
}
