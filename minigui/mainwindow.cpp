#include "mainwindow.h"
#include "ui_mainwindow.h"

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

}

void MainWindow::TestMoteurs()
{

}

void MainWindow::TestOdometrie()
{

}


void MainWindow::PowerHalt()
{
    system("halt");
}

void MainWindow::PowerReboot()
{
    system("reboot");
}

void MainWindow::PowerRestart()
{
    close();
    system("minigui &");
    QApplication::exit();
}
