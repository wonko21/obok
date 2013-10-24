#ifndef OBOK_H
#define OBOK_H

#include <QMap>
#include <QWidget>
#include <QTimer>

class QPushButton;

class Widget : public QWidget
{
    Q_OBJECT

    QString process;
    QTimer  launchTimer;
    QPushButton* pBtnExit;
    QMap<QObject*, QString> data;

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void button();
    void execute();
};

#endif // OBOK_H
