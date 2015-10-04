#include <QFileInfo>
#include <QShortcut>

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

    QObject::connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(ButtonNextPushed()));
    QObject::connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(ButtonPreviousPushed()));

    ShowCurrentImage();

    setWindowTitle("QImageViewer | " + session->GetName());
}

SessionWindow::~SessionWindow()
{
    delete ui;
}

void SessionWindow::closeEvent(QCloseEvent *event)
{
    MainWindow::Get()->RemoveActiveSessionWindow(this);
}

void SessionWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
    {
        ButtonPreviousPushed();
    }
    else if (event->key() == Qt::Key_Right)
    {
        ButtonNextPushed();
    }
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

    QFileInfo info(filename);
    ui->lblFilename->setText(info.fileName());

    QImage image(filename);
    if (image.isNull())
    {
        ui->lblImage->setText("File format not supported");
        return;
    }

    ui->lblImage->setPixmap(QPixmap::fromImage(image));
    ui->lblImage->show();
}
