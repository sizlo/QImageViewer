#include <QFileInfo>
#include <QShortcut>
#include <QMovie>
#include <QDebug>

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
    QObject::connect(ui->btnFirst, SIGNAL(clicked()), this, SLOT(ButtonFirstPushed()));
    QObject::connect(ui->btnLast, SIGNAL(clicked()), this, SLOT(ButtonLastPushed()));

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

void SessionWindow::ButtonFirstPushed()
{
    session->FirstFile();
    ShowCurrentImage();
}

void SessionWindow::ButtonLastPushed()
{
    session->LastFile();
    ShowCurrentImage();
}

void SessionWindow::ShowCurrentImage()
{
    QString filename = session->GetCurrentFileName();

    // Display the current filename in a label
    QFileInfo info(filename);
    ui->lblFilename->setText(info.fileName());

    // Remember the previous movie
    QMovie *oldMovie = ui->lblImage->movie();

    // Setup the movie for the current image
    QMovie *newMovie = new QMovie(filename);
    if (!newMovie->isValid())
    {
        ui->lblImage->setText("File format not supported");
        return;
    }
    ui->lblImage->setMovie(newMovie);
    newMovie->start();

    // Delete the previous movie
    if (oldMovie != NULL)
    {
        delete oldMovie;
    }

    // Set the hover text for the current image
    ui->lblImage->setToolTip(session->GetCurrentHoverText());

    // Pack the window
    ui->lblImage->adjustSize();
    ui->centralwidget->adjustSize();
    this->adjustSize();
}
