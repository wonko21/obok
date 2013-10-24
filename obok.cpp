#include <cstdlib>

#include <QFont>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QDir>

#include "obok.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QFont fnt(font());
    fnt.setPixelSize(30);

    QFont bfnt(fnt);
    bfnt.setBold(true);

    setFont(fnt);

    QVBoxLayout* pLayout = new QVBoxLayout;

    QImage pImage;
    pImage.load(":images/ccc-ffm-bk.png");

    QLabel* pLabel = new QLabel("Applications", this);
    pLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    pLabel->setFont(bfnt);
    pLabel->setPixmap(QPixmap::fromImage(pImage));
    pLayout->addWidget(pLabel);
    pLayout->insertSpacing( -1, 20 );

    QFile f(QCoreApplication::applicationDirPath() + QDir::separator() + "kobo_launcher.ini");
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);
    while ( !stream.atEnd() )
    {
        QString str = stream.readLine();
        QStringList lst = str.split('=');
        if ( lst.size() > 1 )
        {
            QPushButton* pb = new QPushButton(lst[0].trimmed(), this);
            QString path(lst[1].trimmed());
            if ( path[0] != '/' ) // absolute
            {
                path = QCoreApplication::applicationDirPath() + QDir::separator() + path;
            }
            data.insert(pb, path);
            qDebug() << "Added: " << path;
            pLayout->addWidget(pb);
            connect(pb, SIGNAL(clicked()), this, SLOT(button()));
        }
    }

    pLayout->insertSpacing( -1, 20 );

    pBtnExit = new QPushButton( "Exit", this );
    connect(pBtnExit, SIGNAL(clicked()), this, SLOT(close()));
    pLayout->addWidget(pBtnExit);

    pLayout->insertStretch( -1, 1 );

    setLayout(pLayout);

    launchTimer.setInterval(0);
    launchTimer.setSingleShot(true);

    connect( &launchTimer, SIGNAL(timeout()), this, SLOT(execute()) );

//TODO hava makro here to decide between desktop (window) and full screen
    resize(600,800);
    show();
    //showFullScreen();
}

Widget::~Widget()
{
    
}

void Widget::button()
{
    QObject* pSender = sender();
    QMap<QObject*, QString>::Iterator i(data.find(pSender));
    if ( i != data.end() )
    {
        process = i.value();
        launchTimer.start();
    }
}

void Widget::execute()
{
    qDebug() << "Executing: " << process;
    QProcess::startDetached(process);
    process.clear();
}
