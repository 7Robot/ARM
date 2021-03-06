#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void TestMoteurs();
    void TestOdometrie();
    void TestCapteurs();
    void PowerQuit();
    void PowerReboot();
    void PowerRestart();
    void PowerHalt();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
