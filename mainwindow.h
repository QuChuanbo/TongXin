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
    bool TableFindFlag;

protected:
    void keyPressEvent(QKeyEvent *event);


public slots:
    void date_update();
    void file_update();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
