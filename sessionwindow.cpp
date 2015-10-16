#include <QFileInfo>
#include <QShortcut>
#include <QMovie>
#include <QDebug>
#include <QScrollBar>
#include <QSizePolicy>
#include <QDesktopWidget>

#include "sessionwindow.h"
#include "mainwindow.h"
#include "ui_sessionwindow.h"

SessionWindow::SessionWindow(Session *s, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SessionWindow)
{
    session = s;
    // Load the files for the session again, in case any new ones have been added
    session->LoadFiles();

    ui->setupUi(this);

    lblImage = new QLabel;
    lblImage->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    ui->imageScrollArea->setWidget(lblImage);

    QObject::connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(ButtonNextPushed()));
    QObject::connect(ui->btnPrevious, SIGNAL(clicked()), this, SLOT(ButtonPreviousPushed()));
    QObject::connect(ui->btnFirst, SIGNAL(clicked()), this, SLOT(ButtonFirstPushed()));
    QObject::connect(ui->btnLast, SIGNAL(clicked()), this, SLOT(ButtonLastPushed()));

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

void SessionWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    ShowCurrentImage();
}

void SessionWindow::ShowCurrentImage()
{
    QString filename = session->GetCurrentFileName();

    // Display the current filename in a label
    QFileInfo info(filename);
    ui->lblFilename->setText(info.fileName());

    // Remember the previous movie
    QMovie *oldMovie = lblImage->movie();

    // Setup the movie for the current image
    QMovie *newMovie = new QMovie(filename);
    if (!newMovie->isValid())
    {
        lblImage->setText("File format not supported");
        return;
    }
    lblImage->setMovie(newMovie);
    newMovie->start();

    // Delete the previous movie
    if (oldMovie != NULL)
    {
        delete oldMovie;
    }

    // Set the hover text for the current image
    lblImage->setToolTip(session->GetCurrentHoverText());

    // Pack the window
    lblImage->adjustSize();
    // Must resize scroll area by hand, add a small offset so scrollbars hide
    ui->imageScrollArea->setFixedHeight(lblImage->height() + 2);
    ui->imageScrollArea->setFixedWidth(lblImage->width() + 2);
    ui->centralwidget->adjustSize();
    this->adjustSize();

    // If we're too big then shrink the scroll area
    int availableHeight = QApplication::desktop()->availableGeometry().height();
    int availableWidth = QApplication::desktop()->availableGeometry().width();
    int windowHeight = this->frameSize().height();
    int windowWidth = this->frameSize().width();
    if (windowHeight > availableHeight)
    {
        int offset = windowHeight - availableHeight;
        ui->imageScrollArea->setFixedHeight(ui->imageScrollArea->height() - offset);

        // Expand the width because of the new scrollbar
        int roomToGrowWidth = availableWidth - windowWidth;
        int amountToExpandWidth = std::min(roomToGrowWidth, ui->imageScrollArea->verticalScrollBar()->width());
        amountToExpandWidth = std::max(amountToExpandWidth, 0);
        ui->imageScrollArea->setFixedWidth(ui->imageScrollArea->width() + amountToExpandWidth);

        // Pack the rest of the ui
        ui->centralwidget->adjustSize();
        this->adjustSize();
    }

    windowHeight = this->frameSize().height();
    windowWidth = this->frameSize().width();
    if (windowWidth > availableWidth)
    {
        int offset = windowWidth - availableWidth;
        ui->imageScrollArea->setFixedWidth(ui->imageScrollArea->width() - offset);

        // Expand the height because of the new scrollbar
        int roomToGrowHeight = availableHeight - windowHeight;
        int amountToExpandHeight = std::min(roomToGrowHeight, ui->imageScrollArea->horizontalScrollBar()->height());
        amountToExpandHeight = std::max(amountToExpandHeight, 0);
        ui->imageScrollArea->setFixedHeight(ui->imageScrollArea->height() + amountToExpandHeight);

        // Pack the rest of the ui
        ui->centralwidget->adjustSize();
        this->adjustSize();
    }
}
