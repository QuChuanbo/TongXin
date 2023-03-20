#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <focusmanagement.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    FocusManagement * focus_manage;

public slots:
    void onFocusChange(Qt::Key key);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
