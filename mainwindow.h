#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QString>
#include <QDate>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <xlsxdocument.h>
#include <QFile>
#include <QFileDevice>
#include <QFileDialog>
#include <iostream>
#include <string>
#include <QThread>
#include "modbus.h"
#include "uart_config.h"

using namespace std;

class SensorThread : public QThread
{
    Q_OBJECT
public:
    SensorThread();

    ~SensorThread() override;
    modbus_t* pLightFd;

    QString LightResult;

    int AxisFd;
    int ret;
protected:
    void run() override;
private:
    bool bRunExit;
    uint16_t LightLenth[2];
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *watched, QEvent *event);
    QTimer *date_timer;
    QDateTime date_time;
    QString CurDateTime;
    QStringList TimeList;
    QString filePathName;
    QFileInfo fileinfo;
    QMessageBox::StandardButton result;

    int TableRowCount;
    int XlsxRowCount;
    int DeleteRowCount;

protected:
    void keyPressEvent(QKeyEvent *event);


public slots:
    void date_update();
    void file_update();

private:
    Ui::MainWindow *ui;

    SensorThread* pSensorThread;
};

#endif // MAINWINDOW_H
