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
    double width_Factor = (double)(desktop.geometry().width()) / 1920.0 ;
    double height_Factor = (double)(desktop.geometry().height()) / 1080.0;

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
        QPlainTextEdit {font: bold %1px; border: 2px solid #8c8c8c; border-radius: 5px; background-color: white; \
        min-height: %2px; max-height: %2px; min-width: %3px; max-width: %3px; } \
        ").arg(int(16 * height_Factor)).arg(int(76 * height_Factor)).arg(int(296 * width_Factor));
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
        ").arg(int(16 * height_Factor)).arg(int(18 * height_Factor)).arg(int(32 * height_Factor)).arg(int(146 * width_Factor)).arg(int(40 * height_Factor)).arg(int(100 * width_Factor));
    ui->scrollArea_1->setStyleSheet(stylesheet_Widget);

    ui->btnControl->setChecked(true);
    ui->btnControl->setDisabled(true);
    ui->btnSettings->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}

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


