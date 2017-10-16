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
    init_Page();
    init_Serial_Port();
    init_Mode_Button_Mapping();
    init_PID_LineEdit_Mapping();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Initialization */
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
    ui->ptxtStatus_0->setStyleSheet(stylesheet_Widget);

    stylesheet_Widget = QString("\
        QGroupBox { font: %1px; } \
        QGroupBox::title { subcontrol-position: top center; } \
        QLabel { font: bold %2px; } \
        QLineEdit { \
            font: bold %2px; border: 2px solid #8f8f8f; border-radius: 5px; \
            min-height: %3px; min-width: %4px; padding-left: 4px; \
        } \
        QLineEdit:focus { border: 2px solid #cc6600; } \
        QLineEdit:hover { border: 2px solid #cc6600; } \
        QPushButton { font: bold %2px; color: white; background-color: #224d77; border-radius: 5px; min-height: %5px; min-width: %6px} \
        QPushButton:pressed { background-color: #1c4063; } \
        QPushButton:hover:!pressed { background-color: #27598b; } \
        QPlainTextEdit { font: %7px; border: 2px solid #8c8c8c; border-radius: 5px; background-color: white; } \
        ").arg(int(16 * height_Factor)).arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(112 * width_Factor))
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
}

void MainWindow::init_Page()
{
    ui->btnControl->setChecked(true);
    ui->btnControl->setDisabled(true);
    ui->btnSettings->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::init_Serial_Port()
{
    /* combo box baudrate Serial Port */
    ui->cboBaudrate->setCurrentText("115200");

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        /*qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();*/
        ui->cboSerialPort->addItem(info.portName());
    }

    timerSerialPort = new QTimer(this);
    connect(timerSerialPort, SIGNAL(timeout()), this, SLOT(timerSerialPort_timeout()));
    timerSerialPort->start(1000);
}

void MainWindow::init_Mode_Button_Mapping()
{
    /* Mode Button */
    mode_Mapper->setMapping(ui->btnHome, "HOME");
    mode_Mapper->setMapping(ui->btnStop, "STOP");
    mode_Mapper->setMapping(ui->btnManual, "MANUAL");
    mode_Mapper->setMapping(ui->btnPointing, "POINTING");
    mode_Mapper->setMapping(ui->btnTracking, "TRACKING");

    connect(ui->btnHome, SIGNAL(clicked()), mode_Mapper, SLOT(map()));
    connect(ui->btnStop, SIGNAL(clicked()), mode_Mapper, SLOT(map()));
    connect(ui->btnManual, SIGNAL(clicked()), mode_Mapper, SLOT(map()));
    connect(ui->btnPointing, SIGNAL(clicked()), mode_Mapper, SLOT(map()));
    connect(ui->btnTracking, SIGNAL(clicked()), mode_Mapper, SLOT(map()));

    connect(mode_Mapper, SIGNAL(mapped(const QString &)), SLOT(btnMode_clicked(const QString &)));
}

void MainWindow::init_PID_LineEdit_Mapping()
{
    /* Acronym ------------------------------------------------------------------------------------
    **
    ** idx_Axis: 0 - 1
    **      + 0: Azimuth
    **      + 1: Elevator
    **
    **  idx_PID_Name: 0 - 4
    **      + 0: Manual
    **      + 1: Pointing
    **      + 2: Tracking
    **      + 3: Velocity
    **      + 4: Current
    **
    **  idx_Kx: 0 - 4
    **      + 0: Kp
    **      + 1: Ki
    **      + 2: Kd
    **      + 3: Kff1
    **      + 4: Kff2
    **
    ** End of Acronym ---------------------------------------------------------------------------*/


    /* PID LineEdit */
    QSignalMapper *pid_Mapper = new QSignalMapper(this);
    QLineEdit *leditPID;
    QDoubleValidator *pid_Validator = new QDoubleValidator(this);

    pid_Validator->setDecimals(6);
    pid_Validator->setBottom(0.0);

    for (int idx_Axis = 0; idx_Axis < 2; idx_Axis++)
    {
        for (int idx_PID_Name = 0; idx_PID_Name < 5; idx_PID_Name++)
        {
            for (int idx_Kx = 0; idx_Kx < 5; idx_Kx++)
            {
               leditPID =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));

               /* Set Default Value */
               leditPID->setText("0.0");
               setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx] = "0.0";
               changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] = false;

               /* Set Double Validator */
               leditPID->setValidator(pid_Validator);

               /* Set Mapping */
               pid_Mapper->setMapping(leditPID, QString("%1%2%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
               connect(leditPID, SIGNAL(editingFinished()), pid_Mapper, SLOT(map()));
            }
        }
    }
    connect(pid_Mapper, SIGNAL(mapped(const QString &)), SLOT(leditPID_editingFinished(const QString &)));
}

/* Page Buttons */
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

/* Serial Port Signals */
void MainWindow::on_btnConnect_clicked()
{
    QString stylesheet_Widget;

    if (ui->btnConnect->text() == "Connect")
    {
        if (sport_Thread.connect_Port(ui->cboSerialPort->currentText(),
                                      ui->cboBaudrate->currentText().toInt(NULL, 10)) == true)
        {
            ui->btnConnect->setText("Disconnect");
            stylesheet_Widget = QString("\
                QPushButton { font: bold %1px; color: white; background-color: #774122; border-radius: 5px; min-height: %2px;} \
                QPushButton:pressed { background-color: #63361c; } \
                QPushButton:hover:!pressed { background-color: #8b4c27; } \
                ").arg(int(18 * height_Factor)).arg(int(40 * height_Factor));
            ui->btnConnect->setStyleSheet(stylesheet_Widget);
            status_Append_Text("- " + sport_Thread.port_Name() + " is connected");
            timerSerialPort->stop();
            //load_All_Params();
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
        sport_Thread.disconnect_Port();
        status_Append_Text("- " + sport_Thread.port_Name() + " is disconnected");
        timerSerialPort->start(1000);
    }
}

void MainWindow::timerSerialPort_timeout()
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

/* Serial Port functions */
bool MainWindow::serialPort_write(const QByteArray &data)
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
    QByteArray data_Array;
    quint16 checkSum = 0;

    /* Header */
    data_Array.append(QByteArray::fromRawData("\x47\x42\x02\x01\x00", 5));

    /* Length */
    data_Array.append((char)(1 + payload.count() + 2));

    /* MsgID */
    data_Array.append(msgID);

    /* Payload */
    data_Array.append(payload);

    /* Check Sum */
    for (int i = 0; i < data_Array.count(); i++)
    {
        checkSum += static_cast<unsigned char>(data_Array.at(i));
    }
    checkSum = ~checkSum;
    data_Array.append((char)((checkSum >> 8) & 0x0ff));
    data_Array.append((char)(checkSum & 0x0ff));

    return serialPort_write(data_Array);
}

ENUM_SP_STATUS_T MainWindow::send_Command(char msgID, const QByteArray &payload,int wait_Timeout)
{
    return sport_Thread.send_Command_Blocking(msgID, payload, wait_Timeout);
}

/* Load All Params */
bool MainWindow::load_All_Params()
{
    QByteArray data_Array;

    /* Load PID Params */
    for (int idx_Axis = 0; idx_Axis < 2; idx_Axis++)
    {
        for (int idx_PID_Name = 0; idx_PID_Name < 5; idx_PID_Name++)
        {
            data_Array.clear();
            data_Array.append((char)(1 + idx_Axis));
            data_Array.append((char)(1 + idx_PID_Name));

            send_Command(0x0e, data_Array);
        }
    }
    /* Load Mode */

    /* Load Active Axis */

    return true;
}

/* Status Text for ptxtStatus_x */
void MainWindow::status_Append_Text(const QString &text)
{
    ui->ptxtStatus_0->appendPlainText(text);
    ui->ptxtStatus_1->appendPlainText(text);
}

/* Mode Button Signals */
void MainWindow::btnMode_clicked(const QString &cmd)
{
    QByteArray data_Array;
    char msgID;

    /* Set both (AZ and EL) */
    data_Array.clear();
    data_Array.append((char)0x03);

    if (cmd == "HOME")
    {
        msgID = (char)(0x01);
    }
    else if (cmd == "STOP")
    {
        msgID = (char)(0x02);
    }
    else if (cmd == "EMERGENCY_STOP")
    {
        msgID = (char)(0x03);
    }
    else if (cmd == "MANUAL")
    {
        msgID = (char)(0x04);
        data_Array.append((char)0x00);
    }
    else if (cmd == "POINTING")
    {
        msgID = (char)(0x04);
        data_Array.append((char)0x02);
    }
    else if (cmd == "TRACKING")
    {
        msgID = (char)(0x04);
        data_Array.append((char)0x01);
    }

//    if (send_Command(msgID, data_Array, 1) == true)
//        status_Append_Text("- Send: Set " + cmd);
//    else
//        status_Append_Text("- No Serial Port is connected");

    if (sport_Thread.send_Command_Blocking(msgID, data_Array, 2000) != SP_STATUS_ACK_OK)
    {
        ui->btnHome->setChecked(true);
    }
}

/* PID LineEdit & Write Button Signals */
void MainWindow::leditPID_editingFinished(const QString &pid_Name)
{
    QLineEdit *ledit_Sender;
    int idx_Axis, idx_PID_Name, idx_Kx;
    static QString stylesheet_Widget_Changed = QString("\
        QLineEdit { \
            font: bold %1px; border: 2px solid #b3b300; border-radius: 5px; \
            min-height: %2px; min-width: %3px; padding-left: 4px; color: #b3b300; \
        } \
        QLineEdit:focus { border: 2px solid #cccc00; } \
        QLineEdit:hover { border: 2px solid #cccc00; } \
    ").arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(112 * width_Factor));
   static QString stylesheet_Widget_Not_Changed = QString("\
       QLineEdit { \
           font: bold %1px; border: 2px solid #8f8f8f; border-radius: 5px; \
           min-height: %2px; min-width: %3px; padding-left: 4px; \
       } \
       QLineEdit:focus { border: 2px solid #cc6600; } \
       QLineEdit:hover { border: 2px solid #cc6600; } \
   ").arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(112 * width_Factor));

    idx_Axis = pid_Name.at(0).digitValue();
    idx_PID_Name = pid_Name.at(1).digitValue();
    idx_Kx = pid_Name.at(2).digitValue();

    ledit_Sender =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));

    if (setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx] != ledit_Sender->text())
    {
        ledit_Sender->setStyleSheet(stylesheet_Widget_Changed);
        changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] = true;
    }
    else
    {
        ledit_Sender->setStyleSheet(stylesheet_Widget_Not_Changed);
        changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] = false;
    }
}

void MainWindow::on_btnWritePositionLoop_clicked()
{
    QLineEdit *leditPID;
    QByteArray data_Array;
    quint32 scaled_Value = 0;
    bool first_Send_Flag = false;

    ui->btnWritePositionLoop->setDisabled(true);
    for (int idx_Axis = 0; idx_Axis < 2; idx_Axis++)
    {
        for (int idx_PID_Name = 0; idx_PID_Name < 3; idx_PID_Name++)
        {
            for (int idx_Kx = 0; idx_Kx < 5; idx_Kx++)
            {
                leditPID =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                if (leditPID->text() == NULL)
                {
                    status_Append_Text(QString("- Restore Current Value (%1_%2_%3)").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                    leditPID->setText(setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx]);
                    changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] = false;
                }
                if (changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] == true)
                {
                    data_Array.clear();
                    data_Array.append((char)(1 + idx_Axis));
                    data_Array.append((char)(1 + idx_PID_Name));

                    scaled_Value = quint32 (leditPID->text().toDouble() * 1000000);
                    data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
                    data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
                    data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
                    data_Array.append((char)((scaled_Value) & 0x0ff));

                    if (send_Command(0x09 + idx_Kx, data_Array) == true)
                    {
                        if (first_Send_Flag == false)
                        {
                            first_Send_Flag = true;
                            status_Append_Text(QString("- Send: Set Params").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                        }
                    }
                    else
                    {
                        status_Append_Text("- No Serial Port is connected");
                        break;
                    }
                }
            }
        }
    }
    ui->btnWritePositionLoop->setDisabled(false);
}

void MainWindow::on_btnWriteVelocityLoop_clicked()
{
    QLineEdit *leditPID;
    QByteArray data_Array;
    quint32 scaled_Value = 0;
    bool first_Send_Flag = false;

    ui->btnWriteVelocityLoop->setDisabled(true);
    for (int idx_Axis = 0; idx_Axis < 2; idx_Axis++)
    {
        for (int idx_PID_Name = 3; idx_PID_Name < 4; idx_PID_Name++)
        {
            for (int idx_Kx = 0; idx_Kx < 5; idx_Kx++)
            {
                leditPID =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                if (leditPID->text() == NULL)
                {
                    status_Append_Text(QString("- Restore Current Value (%1_%2_%3)").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                    leditPID->setText(setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx]);
                    changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] = false;
                }
                if (changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] == true)
                {
                    data_Array.clear();
                    data_Array.append((char)(1 + idx_Axis));
                    data_Array.append((char)(1 + idx_PID_Name));

                    scaled_Value = quint32 (leditPID->text().toDouble() * 1000000);
                    data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
                    data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
                    data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
                    data_Array.append((char)((scaled_Value) & 0x0ff));

                    if (send_Command(0x09 + idx_Kx, data_Array) == true)
                    {
                        if (first_Send_Flag == false)
                        {
                            first_Send_Flag = true;
                            status_Append_Text(QString("- Send: Set Params").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                        }
                    }
                    else
                    {
                        status_Append_Text("- No Serial Port is connected");
                        break;
                    }
                }
            }
        }
    }
    ui->btnWriteVelocityLoop->setDisabled(false);
}

void MainWindow::on_btnWriteCurrentLoop_clicked()
{
    QLineEdit *leditPID;
    QByteArray data_Array;
    quint32 scaled_Value = 0;
    bool first_Send_Flag = false;

    ui->btnWriteCurrentLoop->setDisabled(true);
    for (int idx_Axis = 0; idx_Axis < 2; idx_Axis++)
    {
        for (int idx_PID_Name = 4; idx_PID_Name < 5; idx_PID_Name++)
        {
            for (int idx_Kx = 0; idx_Kx < 5; idx_Kx++)
            {
                leditPID =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                if (leditPID->text() == NULL)
                {
                    status_Append_Text(QString("- Restore Current Value (%1_%2_%3)").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                    leditPID->setText(setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx]);
                    changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] = false;
                }
                if (changed_PID_LineEdit[idx_Axis][idx_PID_Name][idx_Kx] == true)
                {
                    data_Array.clear();
                    data_Array.append((char)(1 + idx_Axis));
                    data_Array.append((char)(1 + idx_PID_Name));

                    scaled_Value = quint32 (leditPID->text().toDouble() * 1000000);
                    data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
                    data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
                    data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
                    data_Array.append((char)((scaled_Value) & 0x0ff));

                    if (send_Command(0x09 + idx_Kx, data_Array) == true)
                    {
                        if (first_Send_Flag == false)
                        {
                            first_Send_Flag = true;
                            status_Append_Text(QString("- Send: Set Params").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                        }
                    }
                    else
                    {
                        status_Append_Text("- No Serial Port is connected");
                        break;
                    }
                }
            }
        }
    }
    ui->btnWriteCurrentLoop->setDisabled(false);
}

/* Pos and Vel (Set, Get) */
void MainWindow::on_btnAZSetPos_clicked()
{
    QByteArray data_Array;
    qint32 scaled_Value = 0;

    if (ui->leditAZPos->text() == NULL)
    {
        QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        data_Array.clear();
        data_Array.append((char)0x01);

        scaled_Value = qint32 (ui->leditAZPos->text().toDouble() * 100);
        if ((scaled_Value > 18000) || (scaled_Value < -18000))
        {
            QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
        }
        else
        {
            data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
            data_Array.append((char)((scaled_Value) & 0x0ff));

            if (send_Command(0x05, data_Array) == true)
            {
                status_Append_Text("- Send: Set AZ Pos");
            }
            else
            {
                status_Append_Text("- No Serial Port is connected");
            }
        }
    }
}

void MainWindow::on_btnAZSetVel_clicked()
{
    QByteArray data_Array;
    qint32 scaled_Value = 0;

    if (ui->leditAZVel->text() == NULL)
    {
        QToolTip::showText(ui->leditAZVel->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        data_Array.clear();
        data_Array.append((char)0x01);

        scaled_Value = qint32 (ui->leditAZVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditAZVel->mapToGlobal(QPoint()), "Range: (-180, 180)");
//        }
//        else
        {
            data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
            data_Array.append((char)((scaled_Value) & 0x0ff));

            if (send_Command(0x06, data_Array) == true)
            {
                status_Append_Text("- Send: Set AZ Vel");
            }
            else
            {
                status_Append_Text("- No Serial Port is connected");
            }
        }
    }
}

void MainWindow::on_btnAZSetBoth_clicked()
{
    QByteArray data_Array;
    qint32 scaled_Value = 0;

    if (ui->leditAZPos->text() == NULL)
    {
        QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Enter value");
    }
    else if (ui->leditAZVel->text() == NULL)
    {
        QToolTip::showText(ui->leditAZVel->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        data_Array.clear();
        data_Array.append((char)0x01);

        scaled_Value = qint32 (ui->leditAZPos->text().toDouble() * 100);
        if ((scaled_Value > 18000) || (scaled_Value < -18000))
        {
            QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
            return;
        }
        data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
        data_Array.append((char)((scaled_Value) & 0x0ff));

        scaled_Value = qint32 (ui->leditAZVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
//            return;
//        }
        data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
        data_Array.append((char)((scaled_Value) & 0x0ff));

        if (send_Command(0x07, data_Array) == true)
        {
            status_Append_Text("- Send: Set AZ Pos Vel");
        }
        else
        {
            status_Append_Text("- No Serial Port is connected");
        }
    }
}

void MainWindow::on_btnAZGetPos_clicked()
{
    QByteArray data_Array;

    data_Array.clear();
    data_Array.append((char)0x01);
    if (send_Command(0x08, data_Array) == true)
    {
        status_Append_Text("- Send: Get AZ Pos");
    }
    else
    {
        status_Append_Text("- No Serial Port is connected");
    }
}

void MainWindow::on_btnELSetPos_clicked()
{
    QByteArray data_Array;
    qint32 scaled_Value = 0;

    if (ui->leditELPos->text() == NULL)
    {
        QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        data_Array.clear();
        data_Array.append((char)0x02);

        scaled_Value = qint32 (ui->leditELPos->text().toDouble() * 100);
        if ((scaled_Value > 3000) || (scaled_Value < -3000))
        {
            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-30, 30)");
        }
        else
        {
            data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
            data_Array.append((char)((scaled_Value) & 0x0ff));

            if (send_Command(0x05, data_Array) == true)
            {
                status_Append_Text("- Send: Set EL Pos");
            }
            else
            {
                status_Append_Text("- No Serial Port is connected");
            }
        }
    }
}

void MainWindow::on_btnELSetVel_clicked()
{
    QByteArray data_Array;
    qint32 scaled_Value = 0;

    if (ui->leditELVel->text() == NULL)
    {
        QToolTip::showText(ui->leditELVel->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        data_Array.clear();
        data_Array.append((char)0x02);

        scaled_Value = qint32 (ui->leditELVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
//        }
//        else
        {
            data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
            data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
            data_Array.append((char)((scaled_Value) & 0x0ff));

            if (send_Command(0x06, data_Array) == true)
            {
                status_Append_Text("- Send: Set EL Vel");
            }
            else
            {
                status_Append_Text("- No Serial Port is connected");
            }
        }
    }
}

void MainWindow::on_btnELSetBoth_clicked()
{
    QByteArray data_Array;
    qint32 scaled_Value = 0;

    if (ui->leditELPos->text() == NULL)
    {
        QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Enter value");
    }
    else if (ui->leditELVel->text() == NULL)
    {
        QToolTip::showText(ui->leditELVel->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        data_Array.clear();
        data_Array.append((char)0x02);

        scaled_Value = qint32 (ui->leditELPos->text().toDouble() * 100);
        if ((scaled_Value > 18000) || (scaled_Value < -18000))
        {
            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
            return;
        }
        data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
        data_Array.append((char)((scaled_Value) & 0x0ff));

        scaled_Value = qint32 (ui->leditELVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
//            return;
//        }
        data_Array.append((char)((scaled_Value >> 24) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 16) & 0x0ff));
        data_Array.append((char)((scaled_Value >> 8) & 0x0ff));
        data_Array.append((char)((scaled_Value) & 0x0ff));

        if (send_Command(0x07, data_Array) == true)
        {
            status_Append_Text("- Send: Set EL Pos Vel");
        }
        else
        {
            status_Append_Text("- No Serial Port is connected");
        }
    }
}

void MainWindow::on_btnELGetPos_clicked()
{
    QByteArray data_Array;

    data_Array.clear();
    data_Array.append((char)0x02);
    if (send_Command(0x08, data_Array) == true)
    {
        status_Append_Text("- Send: Get EL Pos");
    }
    else
    {
        status_Append_Text("- No Serial Port is connected");
    }
}

/* Active Axis */
void MainWindow::on_btnAZActive_clicked(bool checked)
{
    if (checked == true)
    {

    }
    else
    {

    }
}

void MainWindow::on_btnELActive_clicked(bool checked)
{
    if (checked == true)
    {

    }
    else
    {

    }
}
