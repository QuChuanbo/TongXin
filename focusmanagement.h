#ifndef FOCUSMANAGEMENT_H
#define FOCUSMANAGEMENT_H

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QLine>

class FocusManagement: public QObject
{
    Q_OBJECT

public:
    static FocusManagement* getinstance();
    void startJudge(QWidget* w,Qt::Key key);
    QList<QWidget *> m_eventWidgetList;

signals:
    void sigFocusChange(Qt::Key key);

public slots:
    void onTimeOut();

private:

    explicit FocusManagement(QObject *parent = nullptr);
    void clearInfo();

private:
    QTimer m_timer;
    Qt::Key m_currentKey;

    QMap<int,QWidget *> m_eventMap;
};

#endif // FOCUSMANAGEMENT_H
