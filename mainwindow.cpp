#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include <QStandardPaths>
#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settingsFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    settingsFilePath += QDir::separator();
    settingsFilePath += "settings.ini";
    LoadSettings();

    ui->setupUi(this);

    QObject::connect(ui->btnOpenSession, SIGNAL(clicked()), this, SLOT(ButtonOpenSessionPushed()));
    QObject::connect(ui->btnDeleteSession, SIGNAL(clicked()), this, SLOT(ButtonDeleteSessionPushed()));
    QObject::connect(ui->btnNewSession, SIGNAL(clicked()), this, SLOT(ButtonNewSessionPushed()));

    PopulateSessionList();
}

MainWindow::~MainWindow()
{
    delete ui;

    while (!sessions.empty())
    {
        Session *s = sessions.back();
        sessions.pop_back();
        delete s;
    }
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
        qDebug() << "Opening" << s->GetAsString();
    }
}

void MainWindow::ButtonDeleteSessionPushed()
{
    Session *s = GetCurrentSessionSelection();
    if (s != NULL)
    {
        auto i = sessions.begin();
        while (*i != s) i++;
        sessions.erase(i);
        delete s;
        PopulateSessionList();
        SaveSettings();
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
        SaveSettings();
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
}

Session * MainWindow::GetCurrentSessionSelection()
{
    QItemSelectionModel *selectionModel = ui->listSessions->selectionModel();
    QModelIndexList rows = selectionModel->selectedRows();
    if (rows.empty()) return NULL;
    Session *s = sessions.at(rows.at(0).row());
    return s;
}
