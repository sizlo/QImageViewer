#include <QFileInfo>

#include "sessionwindow.h"
#include "mainwindow.h"
#include "ui_sessionwindow.h"

SessionWindow::SessionWindow(Session *s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SessionWindow)
{
    session = s;
    session->LoadFiles();

    ui->setupUi(this);
    ui->lblImage->setBackgroundRole(QPalette::Base);
    ui->lblImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->lblImage->setScaledContents(true);

    QObject::connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(ButtonNextPushed()));
    QObject::connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(ButtonPreviousPushed()));

    ShowCurrentImage();
}

SessionWindow::~SessionWindow()
{
    delete ui;
}

void SessionWindow::closeEvent(QCloseEvent *event)
{
    MainWindow::Get()->RemoveActiveSessionWindow(this);
}

Session * SessionWindow::GetSession()
{
    return session;
}

void SessionWindow::ButtonNextPushed()
{
    session->NextFile();
    ShowCurrentImage();
}

void SessionWindow::ButtonPreviousPushed()
{
    session->PreviousFile();
    ShowCurrentImage();
}

void SessionWindow::ShowCurrentImage()
{
    QString filename = session->GetCurrentFileName();
    QImage image(filename);
    if (image.isNull())
    {
        qDebug("Broken image");
    }

    ui->lblImage->setPixmap(QPixmap::fromImage(image));
    ui->lblImage->show();

    QFileInfo info(filename);
    ui->lblFilename->setText(info.fileName());

}
