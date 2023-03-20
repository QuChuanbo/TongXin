#include "focusmanagement.h"

FocusManagement *FocusManagement::getinstance()
{
    static FocusManagement * instance = nullptr;
    if(nullptr == instance)
        instance = new FocusManagement();
    return instance;
}

void FocusManagement::startJudge(QWidget *w, Qt::Key key)
{
    if(!w)
        return;
    m_eventWidgetList.push_back(w);
    if(m_timer.isActive())
        return;
    m_timer.start(50);
    m_currentKey = key;
}

void FocusManagement::onTimeOut()
{
    m_timer.stop();
    QWidget* cfw = QApplication::focusWidget();
    if(nullptr == cfw)
    {
        clearInfo();
        return;
    }

    QPoint cfp;
    switch (m_currentKey) {
    case Qt::Key_Up:
        qDebug() << "Key_Up";
        cfp = cfw->mapToGlobal(QPoint(cfw->width()/2,0));
        break;
    case Qt::Key_Down:
        qDebug() << "Key_Down";
        cfp = cfw->mapToGlobal(QPoint(cfw->width()/2,cfw->height()));
        break;
    case Qt::Key_Left:
        qDebug() << "Key_Left";
        cfp = cfw->mapToGlobal(QPoint(0,cfw->height()/2));
        break;
    case Qt::Key_Right:
        qDebug() << "Key_Right";
        cfp = cfw->mapToGlobal(QPoint(cfw->width(),cfw->height()/2));
        break;
    default:
        break;
    }

    for (int i=0;i<m_eventWidgetList.size();i++) {
        QWidget *evw = m_eventWidgetList.at(i);
        if(evw == cfw)
            continue;
        QPoint evp;
        switch (m_currentKey) {
        case Qt::Key_Up:
            qDebug() << "Qt::Key_Up";
            evp = evw->mapToGlobal(QPoint(evw->width()/2,evw->height()));
            if(evp.y()<= cfp.y()){
                m_eventMap[QLineF(evp,cfp).length()] = evw;
            }
            break;
        case Qt::Key_Down:
                qDebug() << "Qt::Key_Down";
                evp = evw->mapToGlobal(QPoint(evw->width()/2,0));
                if(evp.y()>= cfp.y()){
                m_eventMap[QLineF(evp,cfp).length()] = evw;
            }
            break;
        case Qt::Key_Left:
                qDebug() << "Qt::Key_Left";
                evp = evw->mapToGlobal(QPoint(evw->width(),evw->height()/2));
                if(evp.x()- cfp.x()<=0){
                m_eventMap[QLineF(evp,cfp).length()] = evw;
            }
            break;
        case Qt::Key_Right:
                qDebug() << "Qt::Key_Right";
                evp = evw->mapToGlobal(QPoint(0,evw->height()/2));
                if(evp.x()- cfp.x()>=0){
                m_eventMap[QLineF(evp,cfp).length()] = evw;
            }
            break;
        default:
            break;
        }
    }

    if(!m_eventMap.isEmpty()){
        m_eventMap.first()->setFocus();
    }
    clearInfo();
}

void FocusManagement::clearInfo()
{
    m_eventWidgetList.clear();
    m_eventMap.clear();
}

FocusManagement::FocusManagement(QObject *parent) : QObject (parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}
