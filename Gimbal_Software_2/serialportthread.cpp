#include "serialportthread.h"

SerialPortThread::SerialPortThread(QObject *parent)
    : QThread(parent)
{

}

bool SerialPortThread::connect_Port(const QString &port_Name, qint32 baudrate)
{
    this->spt_port_Name = port_Name;
    this->baudrate = baudrate;

    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    serial.setPortName(this->spt_port_Name);
    serial.setBaudRate(this->baudrate);

    return serial.open(QSerialPort::ReadWrite);
}

void SerialPortThread::disconnect_Port()
{
    serial.close();
}

ENUM_ERROR_CODE_T SerialPortThread::transaction(int wait_Timeout, const QByteArray &request)
{
    /* check serial port */
    if (serial.isOpen() == false) return ERROR_CODE_NO_PORT;

    /* check waiting ack */
    if (this->isRunning() == true) return ERROR_CODE_WAITING;

    this->wait_Timeout = wait_Timeout;
    this->request_Data = request;
    this->start();

    return NO_ERROR;
}

void SerialPortThread::run()
{
    mutex.lock();
    this->response_Data.clear();
    /* write request */
    serial.write(this->request_Data);
    if (serial.waitForBytesWritten(this->wait_Timeout))
    {
        /* read response */
        if (serial.waitForReadyRead(this->wait_Timeout))
        {
            this->response_Data = serial.readAll();
            while (serial.waitForReadyRead(10))
                this->response_Data += serial.readAll();

            emit this->response(this->response_Data);
        }
        else
        {
            emit this->timeout("timeout read");
        }
    } else
    {
        emit this->timeout("timeout write");
    }
    mutex.unlock();
}

/* Wrapped Message */
ENUM_SP_STATUS_T SerialPortThread::send_Command_Blocking(char msgID, const QByteArray &payload, int wait_Timeout)
{
    QByteArray request;
    QByteArray response;
    quint16 checkSum = 0;
    ENUM_SP_STATUS_T status = SP_STATUS_ACK_OK;

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
    if (this->response_Data.isNull() == false)
    {
        quint16 checksum = 0, checksum_Mask = 0;

        /* Check DstID */
        if (response.size() < 3) return SP_STATUS_ACK_NOT_OK;
        if (response.at(2) != 0x01) return SP_STATUS_ACK_NOT_OK;

        /* Checksum */
        checksum_Mask = (response.at(response.size() - 2) << 8) & 0x0ff00;
        checksum_Mask += response.at(response.size() - 1) & 0x0ff;
        for (int i = 0; i < (response.size() - 2); i++)
        {
            checksum += static_cast<unsigned char>(response.at(i));
        }
        checksum = ~checksum;
        if (checksum_Mask != checksum) return SP_STATUS_ACK_NOT_OK;

        /* Handle Respond Set Message */
        if (request.at(6) != response.at(6)) return SP_STATUS_ACK_NOT_OK;
        if (request.at(7) != response.at(7)) return SP_STATUS_ACK_NOT_OK;
    }
    return status;
}

