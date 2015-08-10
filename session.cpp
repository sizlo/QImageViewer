#include <QFileInfo>
#include <QDebug>

#include "session.h"
#include "mainwindow.h"

Session::Session(QString filename)
{
    currentFileName = filename;
    QFileInfo info(filename);
    directory = info.absoluteDir();

    currentFileIt = fileNames.begin();
}

QString Session::GetAsString()
{
    return currentFileName;
}

QString Session::GetDirectoryPath()
{
    return directory.absolutePath();
}

QString Session::GetCurrentFileName()
{
    return currentFileName;
}

void Session::LoadFiles()
{
    QFileInfoList fileInfos = directory.entryInfoList(QDir::Files);
    for (auto info: fileInfos)
    {
        fileNames.push_back(info.absoluteFilePath());
    }

    currentFileIt = fileNames.begin();
    while (*currentFileIt != currentFileName)
    {
        currentFileIt++;
    }
}

void Session::NextFile()
{
    currentFileIt++;
    if (currentFileIt == fileNames.end())
    {
        currentFileIt--;
    }
    currentFileName = *currentFileIt;

    MainWindow::Get()->PopulateSessionList();
}

void Session::PreviousFile()
{
    if (currentFileIt != fileNames.begin())
    {
        currentFileIt--;
    }
    currentFileName = *currentFileIt;

    MainWindow::Get()->PopulateSessionList();
}
