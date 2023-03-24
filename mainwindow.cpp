#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->time_year ->installEventFilter(this);
    ui->time_month ->installEventFilter(this);
    ui->time_day ->installEventFilter(this);
    ui->time_shi ->installEventFilter(this);
    ui->time_fen ->installEventFilter(this);
    ui->time_miao ->installEventFilter(this);
    ui->tEdit_Xdu->installEventFilter(this);
    ui->tEdit_Ydu->installEventFilter(this);
    ui->tEdit_ceju->installEventFilter(this);
    ui->tEdit_kowei->installEventFilter(this);
    ui->tEdit_wanzu->installEventFilter(this);
    ui->tEdit_wendu->installEventFilter(this);
    ui->tEdit_zuhao->installEventFilter(this);
    ui->tEdit_kozhij->installEventFilter(this);
    ui->tEdit_wankuo->installEventFilter(this);
    ui->tEdit_zushen->installEventFilter(this);
    ui->tEdit_zuzhij->installEventFilter(this);
    ui->tEdit_wanshen->installEventFilter(this);
    ui->tEdit_sousuojg->installEventFilter(this);
    ui->tEdit_shengshen->installEventFilter(this);
    ui->tEdit_tushen->installEventFilter(this);
    ui->time_year_2 ->installEventFilter(this);
    ui->time_month_2 ->installEventFilter(this);
    ui->time_day_2 ->installEventFilter(this);
    ui->time_shi_2 ->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);

    date_timer = new QTimer(this);
    date_timer->start(1000);
    connect(date_timer,SIGNAL(timeout()),this,SLOT(date_update()));
    TableRowCount = 1;
    XlsxRowCount = 1;
}

void MainWindow::date_update()
{
    date_timer->stop();
    date_time = QDateTime::currentDateTime();
    CurDateTime = date_time.toString("yyyy-MM-dd-hh-mm-ss");
    TimeList = CurDateTime.split("-");
    ui->time_year->setText(*TimeList.begin());
    TimeList.pop_front();
    ui->time_month->setText(*TimeList.begin());
    TimeList.pop_front();
    ui->time_day->setText(*TimeList.begin());
    TimeList.pop_front();
    ui->time_shi->setText(*TimeList.begin());
    TimeList.pop_front();
    ui->time_fen->setText(*TimeList.begin());
    TimeList.pop_front();
    ui->time_miao->setText(*TimeList.begin());
    TimeList.pop_front();
    date_timer->start(500);
}

void MainWindow::file_update()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:

        break;
    case Qt::Key_S:
//        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(TableRowCount);
        ui->tableWidget->setItem(TableRowCount,0,new QTableWidgetItem(QString::number(TableRowCount)));
        ui->tableWidget->setItem(TableRowCount,1,new QTableWidgetItem(date_time.toString("yyyy-MM-dd")));
        ui->tableWidget->setItem(TableRowCount,2,new QTableWidgetItem(date_time.toString("hh:mm:ss")));
        ui->tableWidget->setItem(TableRowCount,3,new QTableWidgetItem(ui->tEdit_zuhao->document()->toPlainText()));
        ui->tableWidget->setItem(TableRowCount,4,new QTableWidgetItem(ui->tEdit_tushen->document()->toPlainText()));
        ui->tableWidget->setItem(TableRowCount,5,new QTableWidgetItem(ui->tEdit_zushen->document()->toPlainText()));
        ui->tableWidget->setItem(TableRowCount,6,new QTableWidgetItem(ui->tEdit_zuzhij->document()->toPlainText()));
        ui->tableWidget->setItem(TableRowCount,7,new QTableWidgetItem(ui->tEdit_kowei->document()->toPlainText()));
        ui->tableWidget->setItem(TableRowCount,8,new QTableWidgetItem(ui->tEdit_kozhij->document()->toPlainText()));
//        TableRowCount++;
//存储到xlsx
        if(ui->tEdit_zuhao->document()->isEmpty() || ui->tEdit_tushen->document()->isEmpty() \
           || ui->tEdit_zushen->document()->isEmpty() || ui->tEdit_zuzhij->document()->isEmpty() \
           || ui->tEdit_kowei->document()->isEmpty() || ui->tEdit_kowei->document()->isEmpty())
        {
            QMessageBox::StandardButton result = QMessageBox::critical(this, "Title","数据填写不全");
        }
        else {
            QString filePathName = "/home/mdtech/QtProject/TongXin/Test.xls";
            //xlsx导出
            QXlsx::Document xlsx(filePathName);
            QStringList titleList;
            // 设置列宽
            xlsx.setColumnWidth(1, 9);//第一行,20列宽

        // 设置保存的默认文件名称
            QFileInfo fileinfo(filePathName);
            if(!fileinfo.isFile())
            {
                // 设置excel表头（第一行数据）
                titleList <<"序号"
                          <<"日期"
                          <<"时间"
                          <<"桩号"
                          <<"入土深度"
                          <<"打桩深度"
                          <<"桩直径"
                          <<"扩孔位置"
                          <<"扩孔直径";

                for (int i = 0; i < 9; i++){
                    xlsx.write(1, i+1, titleList.at(i));//这里写第一行，第n列数据
//                    qDebug() << "TableRowCount 000 = " << TableRowCount;
                }
            }
            else {
                XlsxRowCount = xlsx.dimension().rowCount();
            }
//            qDebug() << "TableRowCount 1111 = " << TableRowCount;
            XlsxRowCount++;
        // 获取保存文件路径
//            QFileDialog *fileDlg = new QFileDialog(this);
//            fileDlg->setWindowTitle("保存文件");
//            fileDlg->setAcceptMode(QFileDialog::AcceptSave);
//            fileDlg->selectFile(defaultFileName);
//            fileDlg->setNameFilter("Excel Files(*.xls *.xlsx)");
//            fileDlg->setDefaultSuffix("xls");

//            if (fileDlg->exec() == QDialog::Accepted)//点击保存
//            {
//                filePathName = fileDlg->selectedFiles().at(0);
//                qDebug() << filePathName;
//            }

            for (int i = 0; i < 9; i++)
            {
//                for(int j = 0;j<ui->tableWidget->rowCount()-1;j++)
//                {
//                    if(ui->tableWidget->item(j+1,i)!=NULL) xlsx.write(j+XlsxRowCount,i+1,ui->tableWidget->item(j+1,i)->text());
//                    else xlsx.write(j+XlsxRowCount,i+1,"");
//                }
                if(ui->tableWidget->item(TableRowCount,i)!=NULL) xlsx.write(XlsxRowCount,i+1,ui->tableWidget->item(TableRowCount,i)->text());
                else xlsx.write(XlsxRowCount,i+1,"");
            }
            TableRowCount++;
            // 最后，保存文件
            xlsx.saveAs(filePathName);
        }

        break;
    case Qt::Key_X:
        ui->stackedWidget->setCurrentIndex(1 - ui->stackedWidget->currentIndex());
        break;
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    QTextEdit* te_watched = (QTextEdit*)watched;
    QKeyEvent* ke_event = (QKeyEvent*)event;
    if((Qt::Key_S == ke_event->key() || Qt::Key_Q == ke_event->key() \
        || Qt::Key_D == ke_event->key() || Qt::Key_X == ke_event->key()) \
        &&(ke_event->KeyPress == ke_event->type()))
    {
        ke_event->ignore();
        return true;
    }

    if(ui->time_year == te_watched)
    {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_month->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_month == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_day->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_year->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_day == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_shi->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_month->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_shi == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_fen->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_day->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_fen == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_miao->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_shi->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_miao == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_ceju->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_fen->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_ceju == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_Xdu->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_miao->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_Xdu == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_wanshen->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_ceju->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_Ydu->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_tushen->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_Ydu == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_wanshen->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_ceju->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_Xdu->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_wanshen == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_shengshen->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_Xdu->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zushen->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_shengshen == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_wanzu->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_wanshen->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zuzhij->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_wanzu == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_wankuo->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_shengshen->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_kowei->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_wankuo == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_wendu->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_wanzu->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_kozhij->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_wendu == te_watched) {
        if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_wankuo->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_kozhij->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_zuhao == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_tushen->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_year->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_ceju->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_tushen == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_zushen->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_Xdu->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_zushen == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_zuzhij->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_tushen->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_wanshen->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_zuzhij == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_kowei->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zushen->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_shengshen->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_kowei == te_watched) {
        if((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_kozhij->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_zuzhij->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_wanzu->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_kozhij == te_watched) {
        if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_kowei->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_wankuo->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if(ui->time_year_2 == te_watched)
    {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_month_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_month_2 == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_day_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_year_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_day_2 == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->time_shi_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_month_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->time_shi_2 == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->tEdit_sousuojg->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_day_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tEdit_sousuojg == te_watched) {
        if((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type()))
        {
            ke_event->ignore();
            ui->Button_dataout->setFocus();
            return true;
        }
        else if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->time_shi_2->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->Button_dataout->hasFocus()) {
        if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
            ui->tEdit_sousuojg->setFocus();
            return true;
        }
        else if ((Qt::Key_Down == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }


    else {
        ke_event->accept();
        return false;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
