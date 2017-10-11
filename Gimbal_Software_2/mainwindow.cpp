#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    /* Initialization */
    init_GUI();
    init_Serial_COM_Port();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_GUI()
{
    QDesktopWidget desktop;
    QString stylesheet_Widget;

    /* Get Screen Resolution */
    width_Factor = (double)(desktop.geometry().width()) / 1920.0 ;
    height_Factor = (double)(desktop.geometry().height()) / 1080.0;

    /* Set GUI Layout */
    stylesheet_Widget = QString("\
        QPushButton { \
            font: bold %1px; color: white; \
            border-radius: 0px; border: none; \
            background-color: #224d77; \
            min-width: %2px; max-width: %2px; \
            min-height: %3px; max-height: %3px; \
        } \
        QPushButton:pressed { background-color: #1c4063; } \
        QPushButton:hover:!pressed:!checked { background-color: #27598b; } \
        QPushButton:checked { color: white; background-color: #2d669f; }\
        ").arg(int(18 * height_Factor)).arg(int(250 * width_Factor)).arg(int(50 * height_Factor));
    ui->btnControl->setStyleSheet(stylesheet_Widget);
    ui->btnControl->setIconSize(QSize(int(50 * width_Factor), int(50 * height_Factor)));
    ui->btnSettings->setStyleSheet(stylesheet_Widget);
    ui->btnSettings->setIconSize(QSize(int(34 * width_Factor), int(34 * height_Factor)));

    stylesheet_Widget = QString("\
        QPushButton { \
            border-radius: 0px; border: none; \
            background-color: #113A61; \
            min-width: %1px; max-width: %1px; \
            min-height: %2px; max-height: %2px; \
        } \
        QPushButton:pressed { background-color: #113A61; } \
        QPushButton:hover:!pressed { background-color: #113A61; }\
        ").arg(int(150 * width_Factor)).arg(int(50 * height_Factor));
    ui->btnGimbalIcon->setStyleSheet(stylesheet_Widget);
    ui->btnGimbalIcon->setIconSize(QSize(int(40 * width_Factor), int(40 * height_Factor)));

    stylesheet_Widget = QString("\
        QGroupBox { font: %1px; min-width: %2px; max-width: %2px; } \
        QGroupBox::title { subcontrol-position: top center; } \
        QLabel { font: bold %3px; } \
        QComboBox { font: bold %3px; border: 2px solid #8c8c8c; border-radius: 5px; min-height: %4px;} \
        QComboBox::drop-down { \
            subcontrol-origin: padding; subcontrol-position: top right; width: 15px; \
            border-left-width: 1px; border-left-color: darkgray; \
            border-left-style: solid; border-top-right-radius: 3px; border-bottom-right-radius: 3px; \
        } \
        QComboBox::down-arrow{ image: url(:/files/images/arrow_down.png); height: 12px; width: 12px; } \
        QComboBox::on { border: 2px solid #cc6600; } \
        QComboBox:hover { border: 2px solid #cc6600; } \
        QComboBox QAbstractItemView { \
            border: 2px solid #cc6600; \
            selection-background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #111, stop: 1 #333); \
        } \
        QPushButton { font: bold %3px; color: white; background-color: #224d77; border-radius: 5px; min-height: %5px;} \
        QPushButton:pressed { background-color: #1c4063; } \
        QPushButton:hover:!pressed { background-color: #27598b; } \
        ").arg(int(16 * height_Factor)).arg(int(300 * height_Factor)).arg(int(18 * height_Factor)).arg(int(36 * height_Factor)).arg(int(40 * height_Factor));
    ui->grpSerialPort->setStyleSheet(stylesheet_Widget);

    stylesheet_Widget = QString("\
        QGroupBox { font: %1px; } \
        QGroupBox::title { subcontrol-position: top center; } \
        QPushButton { font: bold %2px; border: 2px solid #8c8c8c; border-radius: 5px; background-color: white; min-height: %3px; } \
        QPushButton:pressed { background-color: #f2f2f2; } \
        QPushButton:hover:!checked { border: 2px solid #cc6600; } \
        QPushButton:checked { color: white; border: 2px solid #224d77; background-color: #224d77; } \
        ").arg(int(16 * height_Factor)).arg(int(18 * height_Factor)).arg(int(36 * height_Factor));
    ui->grpModeControl->setStyleSheet(stylesheet_Widget);
    ui->grpImageProcessing->setStyleSheet(stylesheet_Widget);

    stylesheet_Widget = QString("\
        QGroupBox { font: %1px; } \
        QGroupBox::title { subcontrol-position: top center; } \
        ").arg(int(16 * height_Factor));
    ui->grpCamera->setStyleSheet(stylesheet_Widget);

    stylesheet_Widget = QString("\
        QPlainTextEdit { \
            font: %1px; border: 2px solid #8c8c8c; border-radius: 5px; background-color: white; \
            min-height: %2px; max-height: %2px; min-width: %3px; max-width: %3px; \
        } \
        ").arg(int(14 * height_Factor)).arg(int(76 * height_Factor)).arg(int(298 * width_Factor));
    ui->ptxtStatus->setStyleSheet(stylesheet_Widget);

    stylesheet_Widget = QString("\
        QGroupBox { font: %1px; } \
        QGroupBox::title { subcontrol-position: top center; } \
        QLabel { font: bold %2px; } \
        QLineEdit { font: bold %2px; border: 2px solid #8f8f8f; border-radius: 5px; min-height: %3px; min-width: %4px} \
        QLineEdit:focus { border: 2px solid #cc6600; } \
        QLineEdit:hover { border: 2px solid #cc6600; } \
        QPushButton { font: bold %2px; color: white; background-color: #224d77; border-radius: 5px; min-height: %5px; min-width: %6px} \
        QPushButton:pressed { background-color: #1c4063; } \
        QPushButton:hover:!pressed { background-color: #27598b; } \
        QPlainTextEdit { font: %7px; border: 2px solid #8c8c8c; border-radius: 5px; background-color: white; } \
        ").arg(int(16 * height_Factor)).arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(146 * width_Factor))
                                .arg(int(40 * height_Factor)).arg(int(100 * width_Factor)).arg(int(14 * height_Factor));
    ui->scrollArea_1->setStyleSheet(stylesheet_Widget);

    stylesheet_Widget = QString("\
        #grpActiveAxis { \
            border: 2px solid gray; border-radius: 10px; font: %1px; \
            min-width: %2px; max-width: %2px; \
        } \
        QPushButton { font: bold %3px; border: 2px solid #8c8c8c; border-radius: 5px; background-color: white; min-height: %4px; color: black; } \
        QPushButton:pressed { background-color: #f2f2f2; } \
        QPushButton:hover:!checked { border: 2px solid #cc6600; } \
        QPushButton:checked { color: white; border: 2px solid #224d77; background-color: #224d77; } \
    ").arg(int(16 * height_Factor)).arg(int(180 * width_Factor)).arg(int(18 * height_Factor)).arg(int(36 * height_Factor));
    ui->grpActiveAxis->setStyleSheet(stylesheet_Widget);


    ui->btnControl->setChecked(true);
    ui->btnControl->setDisabled(true);
    ui->btnSettings->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::init_Serial_COM_Port()
{
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(on_serialPort_readyRead()));

    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        /*qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();*/
        ui->cboSerialPort->addItem(info.portName());
    }

    timerSerialPort = new QTimer(this);
    connect(timerSerialPort, SIGNAL(timeout()), this, SLOT(on_timerSerialPort_timeout()));
    timerSerialPort->start(1000);
}

/* Handle Signal */
void MainWindow::on_btnControl_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnControl->setDisabled(true);
    ui->btnSettings->setDisabled(false);
}

void MainWindow::on_btnSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnSettings->setDisabled(true);
    ui->btnControl->setDisabled(false);
}


void MainWindow::on_btnConnect_clicked()
{
    QString stylesheet_Widget;

    if (ui->btnConnect->text() == "Connect")
    {
        serialPort->setBaudRate(ui->cboBaudrate->currentText().toInt(NULL, 10));
        serialPort->setPortName(ui->cboSerialPort->currentText());

        if (serialPort->open(QSerialPort::ReadWrite) == true)
        {
            ui->btnConnect->setText("Disconnect");
            stylesheet_Widget = QString("\
                QPushButton { font: bold %1px; color: white; background-color: #774122; border-radius: 5px; min-height: %2px;} \
                QPushButton:pressed { background-color: #63361c; } \
                QPushButton:hover:!pressed { background-color: #8b4c27; } \
                ").arg(int(18 * height_Factor)).arg(int(40 * height_Factor));
            ui->btnConnect->setStyleSheet(stylesheet_Widget);
            ui->ptxtStatus->appendPlainText("- " + serialPort->portName() + " is connected");
            timerSerialPort->stop();
        }
    }
    else
    {
        ui->btnConnect->setText("Connect");
        stylesheet_Widget = QString("\
            QPushButton { font: bold %1px; color: white; background-color: #224d77; border-radius: 5px; min-height: %2px;} \
            QPushButton:pressed { background-color: #1c4063; } \
            QPushButton:hover:!pressed { background-color: #27598b; } \
            ").arg(int(18 * height_Factor)).arg(int(40 * height_Factor));
        ui->btnConnect->setStyleSheet(stylesheet_Widget);
        serialPort->close();
        ui->ptxtStatus->appendPlainText("- " + serialPort->portName() + " is disconnected");
        timerSerialPort->start(1000);
    }
}

void MainWindow::on_timerSerialPort_timeout()
{
    list_Serial_Port.clear();

    /* Add new item */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (ui->cboSerialPort->findText(info.portName()) == -1)
        {
            ui->cboSerialPort->addItem(info.portName());
            ui->cboSerialPort->setCurrentText(info.portName());
        }
        list_Serial_Port.append(info.portName());
    }

    /* Remove missing item */
    for (int i = 0; i < ui->cboSerialPort->count(); i++)
    {
        if (list_Serial_Port.contains(ui->cboSerialPort->itemText(i)) == false)
        {
            ui->cboSerialPort->removeItem(i);
        }
    }
}

void MainWindow::on_serialPort_readyRead()
{
    int idx_GB, msg_Length;
    QByteArray received_Msg;

    data_Serial_Port.append(serialPort->readAll());
    idx_GB = data_Serial_Port.indexOf("GB");
    if (idx_GB == -1) return;
    data_Serial_Port.remove(0, idx_GB);
    msg_Length = data_Serial_Port[5] + 6;
    if (data_Serial_Port.size() >= msg_Length)
    {
        received_Msg = data_Serial_Port.left(msg_Length);
        data_Serial_Port.remove(0, msg_Length);
        parse_Msg(received_Msg);
    }
}


void MainWindow::on_btnHome_clicked()
{
    QByteArray dataArray;
    //dataArray.append((char)(1 + ui->cboModeAxis->currentIndex()));
    dataArray.append((char)(0x03));
    if (send_Command(0x01, dataArray) == true)
        ui->ptxtStatus->appendPlainText("- Send: Set Home ");
    else
        ui->ptxtStatus->appendPlainText("- No COM Port is connected");
}
/* Serial COM Port functions */
bool MainWindow::serialCOMPort_write(const QByteArray &data)
{
    if (serialPort->isOpen() == true)
    {
        serialPort->write(data);
        return true;
    }
    return false;
}

bool MainWindow::send_Command(char msgID, const QByteArray &payload)
{
    QByteArray dataArray;
    quint16 checkSum = 0;

    /* Header */
    dataArray.append(QByteArray::fromRawData("\x47\x42\x02\x01\x00", 5));

    /* Length */
    dataArray.append((char)(1 + payload.count() + 2));

    /* MsgID */
    dataArray.append(msgID);

    /* Payload */
    dataArray.append(payload);

    /* Check Sum */
    for (int i = 0; i < dataArray.count(); i++)
    {
        checkSum += static_cast<unsigned char>(dataArray.at(i));
    }
    checkSum = ~checkSum;
    dataArray.append((char)((checkSum >> 8) & 0x0ff));
    dataArray.append((char)(checkSum & 0x0ff));

    return serialCOMPort_write(dataArray);
}

bool MainWindow::parse_Msg(const QByteArray &msg)
{
    QString show_Res_Msg;
    quint16 checksum = 0, checksum_Mask = 0;
    int cur_Value = 0;

    /* Check DstID */
    if (msg.size() < 2) return false;
    if (msg.at(2) != 0x01) return false;

    /* Checksum */
    checksum_Mask = (msg.at(msg.size() - 2) << 8) & 0x0ff00;
    checksum_Mask += msg.at(msg.size() - 1) & 0x0ff;
    for (int i = 0; i < (msg.size() - 2); i++)
    {
        checksum += static_cast<unsigned char>(msg.at(i));
    }
    checksum = ~checksum;
    if (checksum_Mask != checksum) return false;

    /* Handle Respond Set Message */
    show_Res_Msg.clear();
    if (msg.size() < 6) return false;
    switch (msg.at(6))
    {
    case 0x01:
        show_Res_Msg.append("- Receive: Set Home ");
        break;
    case 0x02:
        show_Res_Msg.append("- Receive: Set Stop ");
        break;
    case 0x03:
        show_Res_Msg.append("- Receive: Set Emergency Stop ");
        break;
    case 0x04:
        show_Res_Msg.append("- Receive: Set Mode ");
        break;
    case 0x05:
        show_Res_Msg.append("- Receive: Set Pos ");
        break;
    case 0x06:
        show_Res_Msg.append("- Receive: Set Vel ");
        break;
    case 0x07:
        show_Res_Msg.append("- Receive: Set Pos Vel ");
        break;
    case 0x09:
        show_Res_Msg.append("- Receive: Set Kp ");
        break;
    case 0x0a:
        show_Res_Msg.append("- Receive: Set Ki ");
        break;
    case 0x0b:
        show_Res_Msg.append("- Receive: Set Kd ");
        break;
    case 0x0c:
        show_Res_Msg.append("- Receive: Set Kff1 ");
        break;
    case 0x0d:
        show_Res_Msg.append("- Receive: Set Kff2 ");
        break;
    }

    if (show_Res_Msg.isEmpty() == false)
    {
        if (msg.size() < 8) return false;
        if (msg.at(7) == 0x01) show_Res_Msg.append("AZ ");
        else if (msg.at(7) == 0x02) show_Res_Msg.append("EL ");
        else if (msg.at(7) == 0x03) show_Res_Msg.append("Both ");

        if (msg.at(8) == 0x00) show_Res_Msg.append("Ok");
        else if (msg.at(8) == 0x01) show_Res_Msg.append("Error");

        ui->ptxtStatus->appendPlainText(show_Res_Msg);
        return true;
    }

    /* Handle Respond Get Message */
    switch (msg.at(6))
    {
    case 0x08:
        if (msg.size() < 11) return false;
        cur_Value = (msg.at(8) << 24) & 0x0ff000000;
        cur_Value += (msg.at(9) << 16) & 0x0ff0000;
        cur_Value += (msg.at(10) << 8) & 0x0ff00;
        cur_Value += msg.at(11) & 0x0ff;

        show_Res_Msg.append("- Receive: Get Pos ");

        if (msg.at(7) == 0x01)
        {
            show_Res_Msg.append("AZ ");
            //ui->ledAZPos->setText(QString::number((double)curValue / 100.0, 'f', 2));
        }
        else if (msg.at(7) == 0x02)
        {
            show_Res_Msg.append("EL ");
            //ui->ledELPos->setText(QString::number((double)curValue / 100.0, 'f', 2));
        }

        show_Res_Msg.append("Done");
        break;
    case 0x0e:
        if (msg.size() < 27) return false;
        cur_Value = (msg.at(8) << 24) & 0x0ff000000;
        cur_Value += (msg.at(9) << 16) & 0x0ff0000;
        cur_Value += (msg.at(10) << 8) & 0x0ff00;
        cur_Value += msg.at(11) & 0x0ff;
        //ui->ledKp->setText(QString::number((double)curValue / 1000000.0, 'g', 6));

        cur_Value = (msg.at(12) << 24) & 0x0ff000000;
        cur_Value += (msg.at(13) << 16) & 0x0ff0000;
        cur_Value += (msg.at(14) << 8) & 0x0ff00;
        cur_Value += msg.at(15) & 0x0ff;
        //ui->ledKi->setText(QString::number((double)curValue / 1000000.0, 'g', 6));

        cur_Value = (msg.at(16) << 24) & 0x0ff000000;
        cur_Value += (msg.at(17) << 16) & 0x0ff0000;
        cur_Value += (msg.at(18) << 8) & 0x0ff00;
        cur_Value += msg.at(19) & 0x0ff;
        //ui->ledKd->setText(QString::number((double)curValue / 1000000.0, 'g', 6));

        cur_Value = (msg.at(20) << 24) & 0x0ff000000;
        cur_Value += (msg.at(21) << 16) & 0x0ff0000;
        cur_Value += (msg.at(22) << 8) & 0x0ff00;
        cur_Value += msg.at(23) & 0x0ff;
        //ui->ledKff1->setText(QString::number((double)curValue / 1000000.0, 'g', 6));

        cur_Value = (msg.at(24) << 24) & 0x0ff000000;
        cur_Value += (msg.at(25) << 16) & 0x0ff0000;
        cur_Value += (msg.at(26) << 8) & 0x0ff00;
        cur_Value += msg.at(27) & 0x0ff;
        //ui->ledKff2->setText(QString::number((double)curValue / 1000000.0, 'g', 6));

        show_Res_Msg.append("- Receive: Get Params Done ");
        break;
    }
    ui->ptxtStatus->appendPlainText(show_Res_Msg);
    return true;
}

