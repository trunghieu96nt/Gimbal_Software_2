#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    /* Connect Signals and Slots */
    connect(ui->btnControl, SIGNAL(clicked()), this, SLOT(on_btnControl_custom_clicked()));
    connect(ui->btnSettings, SIGNAL(clicked()), this, SLOT(on_btnSettings_custom_clicked()));

    /* Initialization */
    ui->btnControl->setChecked(true);
    ui->btnControl->setDisabled(true);
    ui->btnSettings->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnControl_custom_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnControl->setDisabled(true);
    ui->btnSettings->setDisabled(false);
}

void MainWindow::on_btnSettings_custom_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnSettings->setDisabled(true);
    ui->btnControl->setDisabled(false);
}
