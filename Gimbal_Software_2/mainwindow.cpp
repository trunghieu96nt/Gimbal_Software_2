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
    init_PID_WR_Button_Mapping();
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
    ui->page_1->setStyleSheet(stylesheet_Widget);

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

    /* QPlainTextEdit Menu */
    ui->ptxtStatus_0->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ptxtStatus_1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ptxtStatus_0,SIGNAL(customContextMenuRequested(QPoint)), this,SLOT(show_ptxtStatus_Menu(QPoint)));
    connect(ui->ptxtStatus_1,SIGNAL(customContextMenuRequested(QPoint)), this,SLOT(show_ptxtStatus_Menu(QPoint)));
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
    /* for send all params function */
    timer_Get_All_Params.setSingleShot(true);
    timer_Get_All_Params.setInterval(50);
    connect(&timer_Get_All_Params, SIGNAL(timeout()), this, SLOT(timer_Get_All_Params_timeout()));

    /* combo box baudrate Serial Port */
    ui->cboBaudrate->setCurrentText("115200");
    connect(&serial_Port, SIGNAL(done(ENUM_SP_STATUS_T,QByteArray,QByteArray)), this, SLOT(serial_port_done(ENUM_SP_STATUS_T,QByteArray,QByteArray)));

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
    /* Defaut value */
    setted_Mode = "";

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

    /* set validator */
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

void MainWindow::init_PID_WR_Button_Mapping()
{
    QSignalMapper *pid_WR_Mapper = new QSignalMapper(this);

    /* Mode Button */
    pid_WR_Mapper->setMapping(ui->btnWritePositionLoop, "POSITION");
    pid_WR_Mapper->setMapping(ui->btnWriteVelocityLoop, "VELOCITY");
    pid_WR_Mapper->setMapping(ui->btnWriteCurrentLoop, "CURRENT");

    connect(ui->btnWritePositionLoop, SIGNAL(clicked()), pid_WR_Mapper, SLOT(map()));
    connect(ui->btnWriteVelocityLoop, SIGNAL(clicked()), pid_WR_Mapper, SLOT(map()));
    connect(ui->btnWriteCurrentLoop, SIGNAL(clicked()), pid_WR_Mapper, SLOT(map()));

    connect(pid_WR_Mapper, SIGNAL(mapped(const QString &)), SLOT(btnWritePID_clicked(const QString &)));
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
        if (serial_Port.connect_Port(ui->cboSerialPort->currentText(),
                                      ui->cboBaudrate->currentText().toInt(NULL, 10)) == true)
        {
            ui->btnConnect->setText("Disconnect");
            stylesheet_Widget = QString("\
                QPushButton { font: bold %1px; color: white; background-color: #774122; border-radius: 5px; min-height: %2px;} \
                QPushButton:pressed { background-color: #63361c; } \
                QPushButton:hover:!pressed { background-color: #8b4c27; } \
                ").arg(int(18 * height_Factor)).arg(int(40 * height_Factor));
            ui->btnConnect->setStyleSheet(stylesheet_Widget);
            status_Append_Text("- " + serial_Port.port_Name() + " is connected");
            timerSerialPort->stop();
            get_All_Params();
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
        serial_Port.disconnect_Port();
        status_Append_Text("- " + serial_Port.port_Name() + " is disconnected");
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

void MainWindow::serial_port_done(ENUM_SP_STATUS_T status, const QByteArray &request, const QByteArray &response)
{
    unsigned char msgID = static_cast<unsigned char>(request.at(6));

    if (status == SP_STATUS_NO_CONNECT)
    {
        status_Append_Text("- No Serial Port is connected", Qt::red);
        return;
    }

    if ((msgID >= 0x01) && (msgID <= 0x04)) //set mode
    {
        QPushButton *button;
        QString cmd;

        switch (msgID)
        {
        case 0x01:
            cmd = "HOME";
            break;
        case 0x02:
            cmd = "STOP";
            break;
        case 0x03:
            cmd = "EMERGENCY_STOP";
            break;
        case 0x04:
            if (request.at(8) == 0x00)
                cmd = "MANUAL";
            else if (request.at(8) == 0x02)
                cmd = "POINTING";
            else
                cmd = "TRACKING";
            break;
        default:
            break;
        }

        if (status != SP_STATUS_RESPONSE_OK)
        {
            /* back to setted mode */
            button = qobject_cast<QPushButton *>(mode_Mapper->mapping(setted_Mode));
            if (button != NULL) button->setChecked(true);
            status_Append_Text("- Fail: Set Mode: " + cmd, Qt::red);
        }
        else //if (status == SP_STATUS_ACK_OK)
        {
            if (response.at(8) == 0x00)
            {
                setted_Mode = cmd;
                status_Append_Text("- Recv: Set Mode " + cmd + " Done", Qt::darkGreen);
            }
            else
            {
                /* back to setted mode */
                button = qobject_cast<QPushButton *>(mode_Mapper->mapping(setted_Mode));
                if (button != NULL) button->setChecked(true);
                status_Append_Text("- Fail: Set Mode " + cmd, Qt::red);
            }
        }
    }
    else if (msgID == 0x05) //get mode
    {
        if (status != SP_STATUS_RESPONSE_OK)
        {
            status_Append_Text("- Fail: Get Mode", Qt::red);
        }
        else //if (status == SP_STATUS_ACK_OK)
        {
            switch (response.at(8))
            {
            case 0x01:
                setted_Mode = "HOME";
                ui->btnHome->setChecked(true);
                break;
            case 0x02:
                setted_Mode = "STOP";
                ui->btnStop->setChecked(true);
                break;
            case 0x03:
                setted_Mode = "EMERGENCY_STOP";
                break;
            case 0x04:
                if (response.at(9) == 0x00)
                {
                    setted_Mode = "MANUAL";
                    ui->btnManual->setChecked(true);
                }
                else if (response.at(9) == 0x02)
                {
                    setted_Mode = "POINTING";
                    ui->btnPointing->setChecked(true);
                }
                else if (response.at(9) == 0x01)
                {
                    setted_Mode = "TRACKING";
                    ui->btnTracking->setChecked(true);
                }
                break;
            default:
                break;
            }
            status_Append_Text("- Recv: Get Mode", Qt::darkGreen);
        }
    }
    else if ((msgID >= 0x06) && (msgID <= 0x08)) // set pos & vel
    {
        QString message_status;

        if (request.at(7) == 0x01) message_status = "AZ";
        else if (request.at(7) == 0x02) message_status = "EL";
        else message_status = "AZ & EL";

        if (msgID == 0x06) message_status.append(" Pos");
        else if (msgID == 0x07) message_status.append(" Vel");
        else if (msgID == 0x08) message_status.append(" Pos & Vel");

        if (status != SP_STATUS_RESPONSE_OK)
        {
            status_Append_Text("- Fail: Set " + message_status, Qt::red);
        }
        else //if (status == SP_STATUS_ACK_OK)
        {
            if (response.at(8) == 0x00)
                status_Append_Text("- Recv: Set " + message_status + " Done", Qt::darkGreen);
            else
                status_Append_Text("- Fail: Set " + message_status, Qt::red);
        }
    }
    else if (msgID == 0x09) //get pos
    {
        QString message_status;

        if (request.at(7) == 0x01) message_status = "AZ";
        else if (request.at(7) == 0x02) message_status = "EL";
        else message_status = "AZ & EL";

        if ((status != SP_STATUS_RESPONSE_OK) || (response.size() < 12))
        {
            status_Append_Text("- Fail: Get " + message_status + " Pos", Qt::red);
        }
        else //if (status == SP_STATUS_ACK_OK)
        {
            int curValue = 0;

            curValue = (response.at(8) << 24) & 0x0ff000000;
            curValue += (response.at(9) << 16) & 0x0ff0000;
            curValue += (response.at(10) << 8) & 0x0ff00;
            curValue += response.at(11) & 0x0ff;

            if (response.at(7) == 0x01)
                ui->leditAZPos->setText(QString::number((double)curValue / 100.0, 'f', 2));
            else if (response.at(7) == 0x02)
                ui->leditELPos->setText(QString::number((double)curValue / 100.0, 'f', 2));

            status_Append_Text("- Recv: Get " + message_status + " Pos Done" , Qt::darkGreen);

        }
    }
    else if ((msgID >= 0x0a) && (msgID <= 0x0e)) //set params pid
    {
        QLineEdit *leditPID;
        QString ledit_Name, message_status;
        int idx_Axis, idx_PID_Name, idx_Kx;
        static QString stylesheet_Widget_Not_Changed = QString("\
            QLineEdit { \
                font: bold %1px; border: 2px solid #8f8f8f; border-radius: 5px; \
                min-height: %2px; min-width: %3px; padding-left: 4px; \
            } \
            QLineEdit:focus { border: 2px solid #cc6600; } \
            QLineEdit:hover { border: 2px solid #cc6600; } \
        ").arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(112 * width_Factor));

        idx_Axis = request.at(7) - 1;
        idx_PID_Name = request.at(8) - 1;
        idx_Kx = request.at(6) - 0x0a;

        if (idx_Axis == 0) message_status = "AZ";
        else if (idx_Axis == 1) message_status = "EL";

        if (idx_PID_Name == 0) message_status += " Manual";
        else if (idx_PID_Name == 1) message_status += " Pointing";
        else if (idx_PID_Name == 2) message_status += " Tracking";
        else if (idx_PID_Name == 3) message_status += " Velocity";
        else if (idx_PID_Name == 4) message_status += " Current";

        if (idx_Kx == 0) message_status += " Kp";
        else if (idx_Kx == 1) message_status += " Ki";
        else if (idx_Kx == 2) message_status += " Kd";
        else if (idx_Kx == 3) message_status += " Kff1";
        else if (idx_Kx == 4) message_status += " Kff2";

        ledit_Name = QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx);

        if (status != SP_STATUS_RESPONSE_OK)
        {
            status_Append_Text("- Fail: Write " + message_status, Qt::red);
            return;
        }
        else
        {
            leditPID = ui->centralWidget->findChild<QLineEdit *>(ledit_Name);
            status_Append_Text("- Recv: Write " + message_status, Qt::darkGreen);
            setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx] = leditPID->text();
            leditPID->setStyleSheet(stylesheet_Widget_Not_Changed);
        }
    }
    else if (msgID == 0x0f) //get params pid
    {
        int cur_Value = 0;
        QLineEdit *ledit_Kx;
        QString message_status;
        int idx_Axis = (request.at(7) - 1);
        int idx_PID_Name = (request.at(8) - 1);
        static QString stylesheet_Widget_Not_Changed = QString("\
            QLineEdit { \
                font: bold %1px; border: 2px solid #8f8f8f; border-radius: 5px; \
                min-height: %2px; min-width: %3px; padding-left: 4px; \
            } \
            QLineEdit:focus { border: 2px solid #cc6600; } \
            QLineEdit:hover { border: 2px solid #cc6600; } \
        ").arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(112 * width_Factor));

        if (idx_Axis == 0x00) message_status = "AZ";
        else if (idx_Axis == 0x01) message_status = "EL";
        else message_status = "AZ & EL";

        if (idx_PID_Name == 0x00) message_status += " Manual";
        else if (idx_PID_Name == 0x01) message_status += " Pointing";
        else if (idx_PID_Name == 0x02) message_status += " Tracking";
        else if (idx_PID_Name == 0x03) message_status += " Velocity";
        else if (idx_PID_Name == 0x04) message_status += " Current";

        if (status != SP_STATUS_RESPONSE_OK)
        {
            status_Append_Text("- Fail: Get PID " + message_status, Qt::red);
        }
        else
        {
            /* Kp */
            cur_Value = (response.at(8) << 24) & 0x0ff000000;
            cur_Value += (response.at(9) << 16) & 0x0ff0000;
            cur_Value += (response.at(10) << 8) & 0x0ff00;
            cur_Value += response.at(11) & 0x0ff;
            ledit_Kx =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_0").arg(idx_Axis).arg(idx_PID_Name));
            ledit_Kx->setText(QString::number((double)cur_Value / 1000000.0, 'g', 6));
            ledit_Kx->setStyleSheet(stylesheet_Widget_Not_Changed);
            setted_PID_Value[idx_Axis][idx_PID_Name][0] = ledit_Kx->text();

            /* Ki */
            cur_Value = (response.at(12) << 24) & 0x0ff000000;
            cur_Value += (response.at(13) << 16) & 0x0ff0000;
            cur_Value += (response.at(14) << 8) & 0x0ff00;
            cur_Value += response.at(15) & 0x0ff;
            ledit_Kx =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_1").arg(idx_Axis).arg(idx_PID_Name));
            ledit_Kx->setText(QString::number((double)cur_Value / 1000000.0, 'g', 6));
            ledit_Kx->setStyleSheet(stylesheet_Widget_Not_Changed);
            setted_PID_Value[idx_Axis][idx_PID_Name][1] = ledit_Kx->text();

            /* Kd */
            cur_Value = (response.at(16) << 24) & 0x0ff000000;
            cur_Value += (response.at(17) << 16) & 0x0ff0000;
            cur_Value += (response.at(18) << 8) & 0x0ff00;
            cur_Value += response.at(19) & 0x0ff;
            ledit_Kx =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_2").arg(idx_Axis).arg(idx_PID_Name));
            ledit_Kx->setText(QString::number((double)cur_Value / 1000000.0, 'g', 6));
            ledit_Kx->setStyleSheet(stylesheet_Widget_Not_Changed);
            setted_PID_Value[idx_Axis][idx_PID_Name][2] = ledit_Kx->text();

            /* Kff1 */
            cur_Value = (response.at(20) << 24) & 0x0ff000000;
            cur_Value += (response.at(21) << 16) & 0x0ff0000;
            cur_Value += (response.at(22) << 8) & 0x0ff00;
            cur_Value += response.at(23) & 0x0ff;
            ledit_Kx =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_3").arg(idx_Axis).arg(idx_PID_Name));
            ledit_Kx->setText(QString::number((double)cur_Value / 1000000.0, 'g', 6));
            ledit_Kx->setStyleSheet(stylesheet_Widget_Not_Changed);
            setted_PID_Value[idx_Axis][idx_PID_Name][3] = ledit_Kx->text();

            /* Kff2 */
            cur_Value = (response.at(24) << 24) & 0x0ff000000;
            cur_Value += (response.at(25) << 16) & 0x0ff0000;
            cur_Value += (response.at(26) << 8) & 0x0ff00;
            cur_Value += response.at(27) & 0x0ff;
            ledit_Kx =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_4").arg(idx_Axis).arg(idx_PID_Name));
            ledit_Kx->setText(QString::number((double)cur_Value / 1000000.0, 'g', 6));
            ledit_Kx->setStyleSheet(stylesheet_Widget_Not_Changed);
            setted_PID_Value[idx_Axis][idx_PID_Name][4] = ledit_Kx->text();

            status_Append_Text("- Recv: Get PID " + message_status, Qt::darkGreen);
        }

    }
    else if (msgID == 0x10) //set active axis
    {
        QString message_status;
        bool *p_Status_Memory;
        QPushButton *button;

        if (request.at(7) == 0x01)
        {
            button = ui->btnAZActive;
            message_status = "AZ";
            p_Status_Memory = &setted_Active_Value[0];
        }
        else //if (request.at(7) == 0x02)
        {
            button = ui->btnELActive;
            message_status = "EL";
            p_Status_Memory = &setted_Active_Value[1];
        }

        if (request.at(8) == 0x00) message_status.append(" Disable");
        else if (request.at(8) == 0x01) message_status.append(" Enable");

        if (status != SP_STATUS_RESPONSE_OK)
        {
            button->setChecked(*p_Status_Memory);
            status_Append_Text("- Fail: Set " + message_status, Qt::red);
        }
        else //if (status == SP_STATUS_ACK_OK)
        {
            if (response.at(8) == 0x00)
            {
                *p_Status_Memory = request.at(8);
                status_Append_Text("- Recv: Set " + message_status + " Done", Qt::darkGreen);
            }
            else
            {
                button->setChecked(*p_Status_Memory);
                status_Append_Text("- Fail: Set " + message_status, Qt::red);
            }
        }
    }
    else if (msgID == 0x11) //get active axis
    {
        QString message_status;

        if (request.at(7) == 0x01) message_status = "AZ";
        else if (request.at(7) == 0x02) message_status = "EL";
        else message_status = "AZ & EL";

        if (status != SP_STATUS_RESPONSE_OK)
        {
            status_Append_Text("- Fail: Get Active " + message_status, Qt::red);
        }
        else //if (status == SP_STATUS_ACK_OK)
        {
            if (response.at(7) == 0x01) //AZ
            {
                if (response.at(8) == 0x01)
                {
                    ui->btnAZActive->setChecked(true);
                    setted_Active_Value[0] = true;
                }
                else
                {
                    ui->btnAZActive->setChecked(false);
                    setted_Active_Value[0] = false;
                }
            }
            else if (response.at(7) == 0x02) //EL
            {
                if (response.at(8) == 0x01)
                {
                    ui->btnELActive->setChecked(true);
                    setted_Active_Value[1] = true;
                }
                else
                {
                    ui->btnELActive->setChecked(false);
                    setted_Active_Value[1] = false;
                }
            }
            status_Append_Text("- Recv: Get Active " + message_status, Qt::darkGreen);
        }
    }

}

/* Load All Params */
bool MainWindow::get_All_Params()
{
    cmd_Counter = 0;
    timer_Get_All_Params.start();
    return true;
}

void MainWindow::timer_Get_All_Params_timeout()
{
    serial_Port.send_Cmd_Non_Blocking(stru_GB_CMD[cmd_Counter].cmd_msgID, stru_GB_CMD[cmd_Counter].cmd_payload);
    cmd_Counter++;
    if (cmd_Counter >= 11)
        cmd_Counter = 0;
    else
        timer_Get_All_Params.start();
}

/* Status Text for ptxtStatus_x */
void MainWindow::status_Append_Text(const QString &text)
{
    ui->ptxtStatus_0->appendPlainText(text);
    ui->ptxtStatus_1->appendPlainText(text);
}

void MainWindow::status_Append_Text(const QString &text, QColor color)
{
    QTextCharFormat tf;

    /* change color to desired color */
    tf.setForeground(QBrush(color));
    ui->ptxtStatus_0->setCurrentCharFormat(tf);
    ui->ptxtStatus_1->setCurrentCharFormat(tf);

    /* append text */
    ui->ptxtStatus_0->appendPlainText(text);
    ui->ptxtStatus_1->appendPlainText(text);

    /* change color to black */
    tf.setForeground(QBrush(Qt::black));
    ui->ptxtStatus_0->setCurrentCharFormat(tf);
    ui->ptxtStatus_1->setCurrentCharFormat(tf);
}

void MainWindow::show_ptxtStatus_Menu(QPoint pt)
{
    QPlainTextEdit *ptxtStatus = qobject_cast<QPlainTextEdit *>(QObject::sender());
    QMenu *ptxtStatus_Menu = ptxtStatus->createStandardContextMenu();
    QAction action_Clear("Clear All", this);
    connect(&action_Clear, SIGNAL(triggered()), this, SLOT(ptxtStatus_Clear()));
    ptxtStatus_Menu->addAction(&action_Clear);
    ptxtStatus_Menu->exec(ptxtStatus->mapToGlobal(pt));
    delete ptxtStatus_Menu;
}

void MainWindow::ptxtStatus_Clear()
{
    ui->ptxtStatus_0->clear();
    ui->ptxtStatus_1->clear();
}

/* Mode Button */
void MainWindow::btnMode_clicked(const QString &cmd)
{
    QByteArray request_Data;
    char msgID;

    /* Set both (AZ and EL) */
    request_Data.clear();
    request_Data.append((char)0x03);

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
        request_Data.append((char)0x00);
    }
    else if (cmd == "POINTING")
    {
        msgID = (char)(0x04);
        request_Data.append((char)0x02);
    }
    else if (cmd == "TRACKING")
    {
        msgID = (char)(0x04);
        request_Data.append((char)0x01);
    }

    status_Append_Text("- Send: Set Mode " + cmd);

    /* Send */
    serial_Port.send_Cmd_Non_Blocking(msgID, request_Data);
}

/* PID LineEdit & Write Button */
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
    }
    else
    {
        ledit_Sender->setStyleSheet(stylesheet_Widget_Not_Changed);
    }
}

/* PID WR Button */
void MainWindow::btnWritePID_clicked(const QString &name)
{
    QLineEdit *leditPID;
    QByteArray request_Data, response_Data;
    quint32 scaled_Value = 0;
    bool first_Send_Flag = false;
    int idx_PID_Name_Min = 0, idx_PID_Name_Max = 0;

    QMutexLocker locker(&mutex_PID_WR);

    if (name == "POSITION")
    {
        idx_PID_Name_Min = 0;
        idx_PID_Name_Max = 3;
    }
    else if (name == "VELOCITY")
    {
        idx_PID_Name_Min = 3;
        idx_PID_Name_Max = 4;
    }
    else //if (name == "CURRENT")
    {
        idx_PID_Name_Min = 4;
        idx_PID_Name_Max = 5;
    }

    for (int idx_Axis = 0; idx_Axis < 2; idx_Axis++)
    {
        for (int idx_PID_Name = idx_PID_Name_Min; idx_PID_Name < idx_PID_Name_Max; idx_PID_Name++)
        {
            for (int idx_Kx = 0; idx_Kx < 5; idx_Kx++)
            {
                leditPID =  ui->centralWidget->findChild<QLineEdit *>(QString("leditPID_%1_%2_%3").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                //qDebug() << leditPID->text();
                if (leditPID->text() == NULL)
                {
                    status_Append_Text(QString("- Restore Current Value (%1_%2_%3)").arg(idx_Axis).arg(idx_PID_Name).arg(idx_Kx));
                    leditPID->setText(setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx]);
                }
                if (leditPID->text() != setted_PID_Value[idx_Axis][idx_PID_Name][idx_Kx])
                {
                    request_Data.clear();
                    request_Data.append((char)(1 + idx_Axis));
                    request_Data.append((char)(1 + idx_PID_Name));

                    scaled_Value = quint32 (leditPID->text().toDouble() * 1000000);
                    request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
                    request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
                    request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
                    request_Data.append((char)((scaled_Value) & 0x0ff));

                    if (first_Send_Flag == false)
                    {
                        first_Send_Flag = true;
                        status_Append_Text("- Send: Write " + name + " Loop");
                    }
                    serial_Port.send_Cmd_Non_Blocking(0x0a + idx_Kx, request_Data);
                }
            }
        }
    }
}

/* Pos and Vel (Set, Get) */
void MainWindow::on_btnAZSetPos_clicked()
{
    QByteArray request_Data;
    qint32 scaled_Value = 0;

    if (ui->leditAZPos->text() == NULL)
    {
        QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        request_Data.clear();
        request_Data.append((char)0x01);

        scaled_Value = qint32 (ui->leditAZPos->text().toDouble() * 100);
        if ((scaled_Value > 18000) || (scaled_Value < -18000))
        {
            QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
        }
        else
        {
            request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
            request_Data.append((char)((scaled_Value) & 0x0ff));

            status_Append_Text("- Send: Set AZ Pos");
            serial_Port.send_Cmd_Non_Blocking(0x06, request_Data);
        }
    }
}

void MainWindow::on_btnAZSetVel_clicked()
{
    QByteArray request_Data;
    qint32 scaled_Value = 0;

    if (ui->leditAZVel->text() == NULL)
    {
        QToolTip::showText(ui->leditAZVel->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        request_Data.clear();
        request_Data.append((char)0x01);

        scaled_Value = qint32 (ui->leditAZVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditAZVel->mapToGlobal(QPoint()), "Range: (-180, 180)");
//        }
//        else
        {
            request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
            request_Data.append((char)((scaled_Value) & 0x0ff));

            status_Append_Text("- Send: Set AZ Vel");
            serial_Port.send_Cmd_Non_Blocking(0x07, request_Data);

        }
    }
}

void MainWindow::on_btnAZSetBoth_clicked()
{
    QByteArray request_Data;
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
        request_Data.clear();
        request_Data.append((char)0x01);

        scaled_Value = qint32 (ui->leditAZPos->text().toDouble() * 100);
        if ((scaled_Value > 18000) || (scaled_Value < -18000))
        {
            QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
            return;
        }
        request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
        request_Data.append((char)((scaled_Value) & 0x0ff));

        scaled_Value = qint32 (ui->leditAZVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditAZPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
//            return;
//        }
        request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
        request_Data.append((char)((scaled_Value) & 0x0ff));

        status_Append_Text("- Send: Set AZ Pos Vel");
        serial_Port.send_Cmd_Non_Blocking(0x08, request_Data);
    }
}

void MainWindow::on_btnAZGetPos_clicked()
{
    QByteArray request_Data;

    request_Data.clear();
    request_Data.append((char)0x01);
    status_Append_Text("- Send: Get AZ Pos");
    serial_Port.send_Cmd_Non_Blocking(0x09, request_Data);
}

void MainWindow::on_btnELSetPos_clicked()
{
    QByteArray request_Data;
    qint32 scaled_Value = 0;

    if (ui->leditELPos->text() == NULL)
    {
        QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        request_Data.clear();
        request_Data.append((char)0x02);

        scaled_Value = qint32 (ui->leditELPos->text().toDouble() * 100);
        if ((scaled_Value > 3000) || (scaled_Value < -3000))
        {
            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-30, 30)");
        }
        else
        {
            request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
            request_Data.append((char)((scaled_Value) & 0x0ff));

            status_Append_Text("- Send: Set EL Pos");
            serial_Port.send_Cmd_Non_Blocking(0x06, request_Data);
        }
    }
}

void MainWindow::on_btnELSetVel_clicked()
{
    QByteArray request_Data;
    qint32 scaled_Value = 0;

    if (ui->leditELVel->text() == NULL)
    {
        QToolTip::showText(ui->leditELVel->mapToGlobal(QPoint()), "Enter value");
    }
    else
    {
        request_Data.clear();
        request_Data.append((char)0x02);

        scaled_Value = qint32 (ui->leditELVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
//        }
//        else
        {
            request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
            request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
            request_Data.append((char)((scaled_Value) & 0x0ff));

            status_Append_Text("- Send: Set EL Vel");
            serial_Port.send_Cmd_Non_Blocking(0x07, request_Data);
        }
    }
}

void MainWindow::on_btnELSetBoth_clicked()
{
    QByteArray request_Data;
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
        request_Data.clear();
        request_Data.append((char)0x02);

        scaled_Value = qint32 (ui->leditELPos->text().toDouble() * 100);
        if ((scaled_Value > 18000) || (scaled_Value < -18000))
        {
            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
            return;
        }
        request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
        request_Data.append((char)((scaled_Value) & 0x0ff));

        scaled_Value = qint32 (ui->leditELVel->text().toDouble() * 100);
//        if ((scaled_Value > 18000) || (scaled_Value < -18000))
//        {
//            QToolTip::showText(ui->leditELPos->mapToGlobal(QPoint()), "Range: (-180, 180)");
//            return;
//        }
        request_Data.append((char)((scaled_Value >> 24) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 16) & 0x0ff));
        request_Data.append((char)((scaled_Value >> 8) & 0x0ff));
        request_Data.append((char)((scaled_Value) & 0x0ff));

        status_Append_Text("- Send: Set EL Pos Vel");
        serial_Port.send_Cmd_Non_Blocking(0x08, request_Data);
    }
}

void MainWindow::on_btnELGetPos_clicked()
{
    QByteArray request_Data;

    request_Data.clear();
    request_Data.append((char)0x02);
    status_Append_Text("- Send: Get EL Pos");
    serial_Port.send_Cmd_Non_Blocking(0x09, request_Data);
}

/* Active Axis */
void MainWindow::on_btnAZActive_clicked(bool checked)
{
    QByteArray request_Data;
    QString message_status;

    request_Data.clear();
    request_Data.append((char)0x01); //Axis AZ


    if (checked == true)
    {
        message_status = "- Send: Set AZ Enable";
        request_Data.append((char)0x01); //enable
    }
    else
    {
        message_status = "- Send: Set AZ Disable";
        request_Data.append((char)0x00); //disable
    }

    status_Append_Text(message_status);
    serial_Port.send_Cmd_Non_Blocking(0x10, request_Data);
}

void MainWindow::on_btnELActive_clicked(bool checked)
{
    QByteArray request_Data;
    QString message_status;

    request_Data.clear();
    request_Data.append((char)0x02); //Axis EL


    if (checked == true)
    {
        message_status = "- Send: Set EL Enable";
        request_Data.append((char)0x01); //enable
    }
    else
    {
        message_status = "- Send: Set EL Disable";
        request_Data.append((char)0x00); //disable
    }

    status_Append_Text(message_status);
    serial_Port.send_Cmd_Non_Blocking(0x10, request_Data);
}
