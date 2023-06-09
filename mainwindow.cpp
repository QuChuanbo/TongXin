#include "mainwindow.h"
#include "ui_mainwindow.h"

SensorThread::SensorThread()
{
    bRunExit = true;
    LightResult = "";
    ret = -1;
    pLightFd = nullptr;
    pAxisFd = nullptr;
    AxisResult[0] = "";
    AxisResult[1] = "";

//    pLightFd = modbus_new_rtu("/dev/ttyUSB0",115200,'N',8,1);
//    if(nullptr == pLightFd)
//    {
//        cout << "Unable to allocate libmodbus contex !" << endl;
//        this->exit();
//        return;
//    }
//    modbus_set_slave(pLightFd, 1);
//    if (-1 == modbus_connect(pLightFd))
//    {
//        cout << "Connection failed:" << modbus_strerror(errno) << endl;
//        this->exit();
//        return;
//    }

    pAxisFd = modbus_new_rtu("/dev/ttyUSB0",9600,'N',8,1);
    if(nullptr == pAxisFd)
    {
        cout << "pAxisFd Unable to allocate libmodbus contex !" << endl;
        this->exit();
        return;
    }
    modbus_set_slave(pAxisFd, 0x50);
    if (-1 == modbus_connect(pAxisFd))
    {
        cout << "Connection failed:" << modbus_strerror(errno) << endl;
        this->exit();
        return;
    }

}

void SensorThread::run()
{
    float Len32,Len32_axis[2];
    while (bRunExit) {
//        ret = modbus_read_registers(pLightFd, 0x10, 0x02, LightLenth);
//        if(-1 == ret)
//        {
//            cout << "pLightFd read error !" << endl;
//        }
//        else {
//            cout << "light[0] = " << LightLenth[0] << " light[1] = " << LightLenth[1] << endl;
//            Len32 = (LightLenth[0]*2^16 + LightLenth[1])/1000;
//            cout << "Len32 = " << Len32 << endl;
//            LightResult = QString::fromStdString(to_string(Len32));
//        }
        /*****************************************/
        ret = modbus_read_registers(pAxisFd, 0x34, 0x0F, AxisLenth);
        if(-1 == ret)
        {
            cout << "pAxisFd read error !" << endl;
        }
        else {
            Len32_axis[0] = static_cast<float>(AxisLenth[9]/32768.0f*180.0f);
            cout << "x = " << Len32_axis[0] << endl;
            Len32_axis[1] = static_cast<float>(AxisLenth[10]/32768.0f*180.0f);
            cout << "y = " << Len32_axis[1] << endl;
            AxisResult[0] = QString::fromStdString(to_string(Len32_axis[0])) + "°";
            AxisResult[1] = QString::fromStdString(to_string(Len32_axis[1])) + "°";
        }


        sleep(1);
    }
}

SensorThread::~SensorThread()
{
    this->bRunExit = false;
    this->wait();
    modbus_close(this->pLightFd);
    modbus_close(this->pAxisFd);
    cout << "SensorThread exit !" << endl;
}

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
//    ui->tEdit_wendu->installEventFilter(this);
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
    TableRowCount = 0;
    XlsxRowCount = 1;
    DeleteRowCount = 0;
    filePathName = "";

    pSensorThread = new SensorThread();
    pSensorThread->start();
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

    ui->tEdit_ceju->setText(this->pSensorThread->LightResult);
    ui->tEdit_Xdu->setText(this->pSensorThread->AxisResult[0]);
    ui->tEdit_Ydu->setText(this->pSensorThread->AxisResult[1]);
    date_timer->start(500);
}

void MainWindow::file_update()
{

}
/*tableWidget的行从0开始，只有数据没有标题*/
/*xlsx的行从1开始，且第一行是标题*/
/*如果tableWidget的行数是0~8(这0~8都是数据)，那么TableRowCount是9，而xlsx的行数是1~10*/
/*当TableRowCount为0时tableWidget无数据*/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
//        ui->tableWidget->clear();
        for (int i=TableRowCount-1;i>=0;i--) {
            ui->tableWidget->removeRow(i);
        }
        filePathName = "/home/mdtech/QtProject/TongXin/Data-";
        filePathName += ui->time_year_2->document()->toPlainText();
        filePathName += "-";
        filePathName += ui->time_month_2->document()->toPlainText();
        filePathName += "-";
        filePathName += ui->time_day_2->document()->toPlainText();
        filePathName += ".xls";
        fileinfo = filePathName;
        if(fileinfo.isFile())
        {
            QXlsx::Document findxlsx(filePathName);
            findxlsx.setColumnWidth(1, 9);
            XlsxRowCount = findxlsx.dimension().rowCount();
            TableRowCount = XlsxRowCount-1;
            for (int i=2;i<=XlsxRowCount;i++) {
                ui->tableWidget->insertRow(i-2);
                ui->tableWidget->setItem(i-2,0,new QTableWidgetItem(findxlsx.cellAt(i,1)->value().toString()));
                ui->tableWidget->setItem(i-2,1,new QTableWidgetItem(findxlsx.cellAt(i,2)->value().toString()));
                ui->tableWidget->setItem(i-2,2,new QTableWidgetItem(findxlsx.cellAt(i,3)->value().toString()));
                ui->tableWidget->setItem(i-2,3,new QTableWidgetItem(findxlsx.cellAt(i,4)->value().toString()));
                ui->tableWidget->setItem(i-2,4,new QTableWidgetItem(findxlsx.cellAt(i,5)->value().toString()));
                ui->tableWidget->setItem(i-2,5,new QTableWidgetItem(findxlsx.cellAt(i,6)->value().toString()));
                ui->tableWidget->setItem(i-2,6,new QTableWidgetItem(findxlsx.cellAt(i,7)->value().toString()));
                ui->tableWidget->setItem(i-2,7,new QTableWidgetItem(findxlsx.cellAt(i,8)->value().toString()));
                ui->tableWidget->setItem(i-2,8,new QTableWidgetItem(findxlsx.cellAt(i,9)->value().toString()));
            }
        }
        else {
            result = QMessageBox::critical(this, "Title","未查找到数据");
        }
        break;
    case Qt::Key_S:
        if(ui->tEdit_zuhao->document()->isEmpty() || ui->tEdit_tushen->document()->isEmpty() \
           || ui->tEdit_zushen->document()->isEmpty() || ui->tEdit_zuzhij->document()->isEmpty())
        {
            result = QMessageBox::critical(this, "Title","数据填写不全");
        }
        else {
            filePathName = "/home/mdtech/QtProject/TongXin/Data-";
            filePathName += date_time.toString("yyyy-MM-dd");
            filePathName += ".xls";
            //xlsx导出
            QXlsx::Document xlsx(filePathName);
            QStringList titleList;
            // 设置列宽
            xlsx.setColumnWidth(1, 9);//第一行,20列宽

        // 设置保存的默认文件名称
            fileinfo = filePathName;
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
                TableRowCount = ui->tableWidget->rowCount() - 1;
                for (int i=TableRowCount-1;i>=0;i--) {
                    ui->tableWidget->removeRow(i);
                }
                TableRowCount = XlsxRowCount-1;
                for (int i=2;i<=XlsxRowCount;i++) {
                    ui->tableWidget->insertRow(i-2);
                    ui->tableWidget->setItem(i-2,0,new QTableWidgetItem(xlsx.cellAt(i,1)->value().toString()));
                    ui->tableWidget->setItem(i-2,1,new QTableWidgetItem(xlsx.cellAt(i,2)->value().toString()));
                    ui->tableWidget->setItem(i-2,2,new QTableWidgetItem(xlsx.cellAt(i,3)->value().toString()));
                    ui->tableWidget->setItem(i-2,3,new QTableWidgetItem(xlsx.cellAt(i,4)->value().toString()));
                    ui->tableWidget->setItem(i-2,4,new QTableWidgetItem(xlsx.cellAt(i,5)->value().toString()));
                    ui->tableWidget->setItem(i-2,5,new QTableWidgetItem(xlsx.cellAt(i,6)->value().toString()));
                    ui->tableWidget->setItem(i-2,6,new QTableWidgetItem(xlsx.cellAt(i,7)->value().toString()));
                    ui->tableWidget->setItem(i-2,7,new QTableWidgetItem(xlsx.cellAt(i,8)->value().toString()));
                    ui->tableWidget->setItem(i-2,8,new QTableWidgetItem(xlsx.cellAt(i,9)->value().toString()));
                }
            }

            ui->tableWidget->insertRow(TableRowCount);
            ui->tableWidget->setItem(TableRowCount,0,new QTableWidgetItem(QString::number(TableRowCount+1)));
            ui->tableWidget->setItem(TableRowCount,1,new QTableWidgetItem(date_time.toString("yyyy-MM-dd")));
            ui->tableWidget->setItem(TableRowCount,2,new QTableWidgetItem(date_time.toString("hh:mm:ss")));
            ui->tableWidget->setItem(TableRowCount,3,new QTableWidgetItem(ui->tEdit_zuhao->document()->toPlainText()));
            ui->tableWidget->setItem(TableRowCount,4,new QTableWidgetItem(ui->tEdit_tushen->document()->toPlainText()));
            ui->tableWidget->setItem(TableRowCount,5,new QTableWidgetItem(ui->tEdit_zushen->document()->toPlainText()));
            ui->tableWidget->setItem(TableRowCount,6,new QTableWidgetItem(ui->tEdit_zuzhij->document()->toPlainText()));
            ui->tableWidget->setItem(TableRowCount,7,new QTableWidgetItem(ui->tEdit_kowei->document()->toPlainText()));
            ui->tableWidget->setItem(TableRowCount,8,new QTableWidgetItem(ui->tEdit_kozhij->document()->toPlainText()));

//            qDebug() << "TableRowCount 1111 = " << TableRowCount;
            XlsxRowCount++;
            for (int i = 0; i < 9; i++)
            {
                if(ui->tableWidget->item(TableRowCount,i)!=NULL)
                    xlsx.write(XlsxRowCount,i+1,ui->tableWidget->item(TableRowCount,i)->text());
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
    case Qt::Key_M:
        if("" == filePathName)
        {
            result = QMessageBox::critical(this, "Title","未选择数据无法修改");
            break;
        }
        fileinfo = filePathName;
        if(fileinfo.isFile())
            fileinfo.dir().remove(fileinfo.fileName());
        {
        QXlsx::Document motifyxlsx(filePathName);
        QStringList titleList;
        // 设置列宽
        motifyxlsx.setColumnWidth(1, 9);//第一行,20列宽

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
            motifyxlsx.write(1, i+1, titleList.at(i));//这里写第一行，第n列数据
        }

        TableRowCount = ui->tableWidget->rowCount() - 1;
        qDebug() << "TableRowCount = " << TableRowCount;
        XlsxRowCount = TableRowCount + 1;
        for (int i=2;i<=XlsxRowCount;i++) {
            for (int j=0;j<9;j++) {
                motifyxlsx.write(i, j+1, ui->tableWidget->item(i-2,j)->text());
            }
        }
        motifyxlsx.saveAs(filePathName);
        }
        break;

    case Qt::Key_Minus:

        if("" == filePathName)
        {
            result = QMessageBox::critical(this, "Title","未插入数据无法删除");
            break;
        }

        filePathName = "/home/mdtech/QtProject/TongXin/Data-";
        filePathName += date_time.toString("yyyy-MM-dd");
        filePathName += ".xls";
        fileinfo = filePathName;
        if(!fileinfo.isFile())
        {
            result = QMessageBox::critical(this, "Title","当日Excel数据丢失无法删除");
            break;
        }
        //xlsx导出
        QXlsx::Document readxlsx(filePathName);

        readxlsx.setColumnWidth(1, 9);
        XlsxRowCount = readxlsx.dimension().rowCount();
        if(2 > XlsxRowCount)
        {
            result = QMessageBox::critical(this, "Title","当日Excel无数据无法删除");
            break;
        }
//        for (int i = 0; i < 9; i++){
//            deletexlsx.write(XlsxRowCount, i+1, "");//这里写第一行，第n列数据
//        }

        TableRowCount = ui->tableWidget->rowCount() - 1;
        for (int i=TableRowCount-1;i>=0;i--) {
            ui->tableWidget->removeRow(i);
        }

//        TableRowCount = XlsxRowCount-2;//最后一行不复制
        for (int i=2;i<=XlsxRowCount-1;i++) {
            ui->tableWidget->insertRow(i-2);
            ui->tableWidget->setItem(i-2,0,new QTableWidgetItem(readxlsx.cellAt(i,1)->value().toString()));
            ui->tableWidget->setItem(i-2,1,new QTableWidgetItem(readxlsx.cellAt(i,2)->value().toString()));
            ui->tableWidget->setItem(i-2,2,new QTableWidgetItem(readxlsx.cellAt(i,3)->value().toString()));
            ui->tableWidget->setItem(i-2,3,new QTableWidgetItem(readxlsx.cellAt(i,4)->value().toString()));
            ui->tableWidget->setItem(i-2,4,new QTableWidgetItem(readxlsx.cellAt(i,5)->value().toString()));
            ui->tableWidget->setItem(i-2,5,new QTableWidgetItem(readxlsx.cellAt(i,6)->value().toString()));
            ui->tableWidget->setItem(i-2,6,new QTableWidgetItem(readxlsx.cellAt(i,7)->value().toString()));
            ui->tableWidget->setItem(i-2,7,new QTableWidgetItem(readxlsx.cellAt(i,8)->value().toString()));
            ui->tableWidget->setItem(i-2,8,new QTableWidgetItem(readxlsx.cellAt(i,9)->value().toString()));
        }
        readxlsx.saveAs(filePathName);
        fileinfo.dir().remove(fileinfo.fileName());

        QXlsx::Document deletexlsx(filePathName);
        QStringList titleList;
        deletexlsx.setColumnWidth(1, 9);

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
            deletexlsx.write(1, i+1, titleList.at(i));//这里写第一行，第n列数据
        }

        TableRowCount = ui->tableWidget->rowCount() - 1;
        qDebug() << "TableRowCount = " << TableRowCount;
        XlsxRowCount = TableRowCount + 1;
        for (int i=2;i<=XlsxRowCount;i++) {
            for (int j=0;j<9;j++) {
                deletexlsx.write(i, j+1, ui->tableWidget->item(i-2,j)->text());
            }
        }

        deletexlsx.saveAs(filePathName);
        break;

    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    QTextEdit* te_watched = (QTextEdit*)watched;
    QKeyEvent* ke_event = (QKeyEvent*)event;
    if((Qt::Key_S == ke_event->key() || Qt::Key_Q == ke_event->key() || Qt::Key_M == ke_event->key() \
        || Qt::Key_D == ke_event->key() || Qt::Key_X == ke_event->key() \
        || Qt::Key_Plus == ke_event->key() || Qt::Key_Minus == ke_event->key()) \
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
        if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
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
            ui->tableWidget->setFocus();
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
            ui->tableWidget->setFocus();
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
            ui->tableWidget->setFocus();
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
            ui->tableWidget->setFocus();
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
            ui->tableWidget->setFocus();
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
            ui->tableWidget->setFocus();
            return true;
        }
        else {
            ke_event->accept();
            return false;
        }
    }
    else if (ui->tableWidget->hasFocus()) {
        if ((Qt::Key_Left == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_sousuojg->setFocus();
            return true;
        }
        else if ((Qt::Key_Right == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
            return true;
        }
        else if ((Qt::Key_Up == ke_event->key())&&(ke_event->KeyPress == ke_event->type())) {
            ke_event->ignore();
//            ui->tEdit_zuhao->setFocus();
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
    delete pSensorThread;
}
