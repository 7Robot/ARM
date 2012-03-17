#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::TestCapteurs()
{
    QMessageBox msgBox;
    msgBox.setText("Test capteurs.");
    msgBox.exec();
}

void MainWindow::TestMoteurs()
{
    QMessageBox msgBox;
    msgBox.setText("Test moteurs.");
    msgBox.exec();
}

void MainWindow::TestOdometrie()
{
    QMessageBox msgBox;
    msgBox.setText("Test odométrie.");
    msgBox.exec();
}

void MainWindow::PowerQuit()
{
    close();
    QApplication::exit();
}

void MainWindow::PowerRestart()
{
    PowerQuit();
    system("minigui &"); // TODO: -qws ?
}

void MainWindow::PowerHalt()
{
    system("halt");
}

void MainWindow::PowerReboot()
{
    system("reboot");
}
