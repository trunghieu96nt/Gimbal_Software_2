#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    timerSend.setSingleShot(true);
    timerSend.setInterval(2000);
    connect(&timerSend, SIGNAL(timeout()), this, SLOT(timerSendTimeout()));

    connect(&serial, SIGNAL(readyRead()), this, SLOT(serialReadyRead()));
}

/* open and close port */
bool SerialPort::connectPort(const QString &port_Name, qint32 baudrate)
{
    this->spPortName = port_Name;
    this->spBaudrate = baudrate;

    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    serial.setPortName(this->spPortName);
    serial.setBaudRate(this->spBaudrate);

    return serial.open(QSerialPort::ReadWrite);
}

void SerialPort::disconnectPort()
{
    serial.close();
}

/* send and receive */
void SerialPort::transaction(const QByteArray &request)
{
    this->requestQueue.enqueue(request);

    if (timerSend.isActive() == false)
    {
        this->currentRequest = this->requestQueue.dequeue();
        if (serial.isOpen() == false)
        {
            emit this->done(SP_STATUS_NO_CONNECT, this->currentRequest, NULL);
            this->currentRequest.clear();
            this->requestQueue.clear();
        }
        else
        {
            serial.write(this->currentRequest);
            if (serial.waitForBytesWritten(10))
            {
                timerSend.start();
            }
            else
            {
                emit this->done(SP_STATUS_TIMEOUT_WR, this->currentRequest, NULL);
                this->currentRequest.clear();
                this->requestQueue.clear();
            }
        }
    }
}

void SerialPort::transactionInQueue()
{
    if (this->requestQueue.isEmpty() == false)
    {
        this->currentRequest = this->requestQueue.dequeue();
        if (serial.isOpen() == false)
        {
            emit this->done(SP_STATUS_NO_CONNECT, this->currentRequest, NULL);
            this->currentRequest.clear();
            this->requestQueue.clear();
        }
        else
        {
            serial.write(this->currentRequest);
            if (serial.waitForBytesWritten(10))
            {
                timerSend.start();
            }
            else
            {
                emit this->done(SP_STATUS_TIMEOUT_WR, this->currentRequest, NULL);
                this->currentRequest.clear();
                this->requestQueue.clear();
            }
        }
    }
}

void SerialPort::timerSendTimeout()
{
    emit this->done(SP_STATUS_TIMEOUT_RD, this->currentRequest, NULL);
    this->currentRequest.clear();
    this->transactionInQueue();
}

void SerialPort::serialReadyRead()
{
    int idx_GB, msg_Length;
    QByteArray response;

    this->dataSerialPort.append(serial.readAll());
    idx_GB = dataSerialPort.indexOf("GB");
    if (idx_GB == -1) return;

    /* remove invalid data */
    dataSerialPort.remove(0, idx_GB);

    /* wait size until greater than 6 */
    if (dataSerialPort.size() < 6) return;
    msg_Length = dataSerialPort.at(5) + 6;

    /* wait size until greater than msg_Length */
    if (dataSerialPort.size() < msg_Length) return;

    /* get response */
    response = dataSerialPort.left(msg_Length);
    dataSerialPort.remove(0, msg_Length);

    /* check response */
    if (this->currentRequest.isNull() == true) return;

    timerSend.stop();
    if (response.size() < 10)
    {
        emit this->done(SP_STATUS_RESPONSE_ERR, this->currentRequest, NULL);
        this->currentRequest.clear();
    }
    else
    {
        quint16 checksum = 0, checksum_Mask = 0;
        checksum_Mask = (response.at(response.size() - 2) << 8) & 0x0ff00;
        checksum_Mask += response.at(response.size() - 1) & 0x0ff;
        for (int i = 0; i < (response.size() - 2); i++)
        {
            checksum += static_cast<unsigned char>(response.at(i));
        }
        checksum = ~checksum;

        if ((checksum_Mask != checksum) || (response.at(2) != 0x01) ||
                (response.at(6) != this->currentRequest.at(6)) || (response.at(7) != this->currentRequest.at(7)))
        {
            emit this->done(SP_STATUS_RESPONSE_ERR, this->currentRequest, NULL);
            this->currentRequest.clear();
        }
        else
        {
            emit this->done(SP_STATUS_RESPONSE_OK, this->currentRequest, response);
            this->currentRequest.clear();
        }
    }
    this->transactionInQueue();
}

/* Packaged Send */
ENUM_SP_STATUS_T SerialPort::sendCmdBlocking(char msgID, const QByteArray &payload, int wait_Timeout, QByteArray &response)
{
    QByteArray request;
    quint16 checkSum = 0;
    ENUM_SP_STATUS_T status = SP_STATUS_RESPONSE_OK;

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
        mutexSend.lock();
        disconnect(&serial, SIGNAL(readyRead()), this, SLOT(serialReadyRead()));
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
        connect(&serial, SIGNAL(readyRead()), this, SLOT(serialReadyRead()));
        mutexSend.unlock();
    }

    /* check ack */
    if (response.isNull() == false)
    {
        quint16 checksum = 0, checksum_Mask = 0;

        /* Check DstID */
        if (response.size() < 3) return SP_STATUS_RESPONSE_ERR;
        if (response.at(2) != 0x01) return SP_STATUS_RESPONSE_ERR;

        /* Checksum */
        checksum_Mask = (response.at(response.size() - 2) << 8) & 0x0ff00;
        checksum_Mask += response.at(response.size() - 1) & 0x0ff;
        for (int i = 0; i < (response.size() - 2); i++)
        {
            checksum += static_cast<unsigned char>(response.at(i));
        }
        checksum = ~checksum;
        if (checksum_Mask != checksum) return SP_STATUS_RESPONSE_ERR;

        /* Handle Response Set Message */
        if (request.at(6) != response.at(6)) return SP_STATUS_RESPONSE_ERR;
        if (request.at(7) != response.at(7)) return SP_STATUS_RESPONSE_ERR;
    }
    return status;
}

void SerialPort::sendCmdNonBlocking(char msgID, const QByteArray &payload)
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
