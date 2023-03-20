#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    focus_manage = FocusManagement::getinstance();
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_Xdu);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_Ydu);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_ceju);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_kowei);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_wanzu);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_wendu);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_zuhao);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_kozhij);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_wankuo);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_zushen);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_sousuojg);
    focus_manage->m_eventWidgetList.push_back(ui->tEdit_shengshen);

    connect(focus_manage,SIGNAL(sigFocusChange(Qt::key)),this, \
    SLOT(onFocusChange(Qt::key)));
}

void MainWindow::onFocusChange(Qt::Key key)
{
    for(int i = 0; i<focus_manage->m_eventWidgetList.size();i++)
    {
        FocusManagement::getinstance()->startJudge(static_cast<QWidget*> \
        (focus_manage->m_eventWidgetList.at(i)),key);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
