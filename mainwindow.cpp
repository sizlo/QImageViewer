#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include <QStandardPaths>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow * MainWindow::instance  = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    instance = this;

    settingsFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settingsFilePath += QDir::separator();
    settingsFilePath += "settings.ini";
    LoadSettings();

    ui->setupUi(this);

    QObject::connect(ui->btnOpenSession, SIGNAL(clicked()), this, SLOT(ButtonOpenSessionPushed()));
    QObject::connect(ui->btnDeleteSession, SIGNAL(clicked()), this, SLOT(ButtonDeleteSessionPushed()));
    QObject::connect(ui->btnNewSession, SIGNAL(clicked()), this, SLOT(ButtonNewSessionPushed()));

    PopulateSessionList();

    setWindowTitle("QImageViewer");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    if (!activeSessionWindows.empty())
    {
        if (QMessageBox::Yes == QMessageBox::question(this, "Sessions still active",
                                      "Are you sure you want to exit and close all sessions?",
                                      QMessageBox::Yes|QMessageBox::No))
        {
            while (!activeSessionWindows.empty())
            {
                SessionWindow *w = activeSessionWindows.back();
                activeSessionWindows.pop_back();
                w->close();
                delete w;
            }

            while (!sessions.empty())
            {
                Session *s = sessions.back();
                sessions.pop_back();
                delete s;
            }
        }
        else
        {
            event->ignore();
        }
    }

}

MainWindow * MainWindow::Get()
{
    return instance;
}

void MainWindow::LoadSettings()
{
    QSettings settings(settingsFilePath, QSettings::NativeFormat);
    QStringList sessionFiles = settings.value("sessionList", QStringList()).toStringList();
    for (auto file: sessionFiles)
    {
        Session *s = new Session(file);
        sessions.push_back(s);
    }
}

void MainWindow::SaveSettings()
{
    QSettings settings(settingsFilePath, QSettings::NativeFormat);
    settings.setValue("sessionList", GetSessionNames());
}

void MainWindow::ButtonOpenSessionPushed()
{
    Session *s = GetCurrentSessionSelection();
    if (s != NULL)
    {
        for (auto w: activeSessionWindows)
        {
            if (w->GetSession() == s)
            {
                w->activateWindow();
                w->raise();
                return;
            }
        }

        SessionWindow *w = new SessionWindow(s);
        activeSessionWindows.push_back(w);
        w->show();
    }
}

void MainWindow::ButtonDeleteSessionPushed()
{
    Session *s = GetCurrentSessionSelection();
    if (s != NULL)
    {
        bool shouldDelete = true;
        for (auto w: activeSessionWindows)
        {
            if (w->GetSession() == s)
            {
                if (QMessageBox::Yes == QMessageBox::question(this, "Session active",
                                              "Are you sure you want to delete this active session?",
                                              QMessageBox::Yes|QMessageBox::No))
                {
                    w->close();
                }
                else
                {
                    shouldDelete = false;
                }
                break;
            }
        }

        if (shouldDelete)
        {
            auto i = sessions.begin();
            while (*i != s) i++;
            sessions.erase(i);
            delete s;
            PopulateSessionList();
        }
    }
}

void MainWindow::ButtonNewSessionPushed()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
    {
        QString filename = dialog.selectedFiles().at(0);
        Session *newSession = new Session(filename);
        sessions.push_back(newSession);
        PopulateSessionList();
    }
}

QStringList MainWindow::GetSessionNames()
{
    QStringList sessionNames;
    for (auto s: sessions)
    {
        sessionNames.append(s->GetAsString());
    }
    return sessionNames;
}

void MainWindow::PopulateSessionList()
{
    QStringList sessionNames = GetSessionNames();
    QAbstractItemModel *oldModel = ui->listSessions->model();
    delete oldModel;
    QStringListModel *newModel = new QStringListModel(sessionNames);
    ui->listSessions->setModel(newModel);

    SaveSettings();
}

Session * MainWindow::GetCurrentSessionSelection()
{
    QItemSelectionModel *selectionModel = ui->listSessions->selectionModel();
    QModelIndexList rows = selectionModel->selectedRows();
    if (rows.empty()) return NULL;
    Session *s = sessions.at(rows.at(0).row());
    return s;
}

void MainWindow::RemoveActiveSessionWindow(SessionWindow *w)
{
    activeSessionWindows.remove(w);
}
