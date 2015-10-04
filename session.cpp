#include <QFileInfo>
#include <QDebug>

#include "session.h"
#include "mainwindow.h"

Session::Session(QString descriptor)
{
    QStringList items = descriptor.split(";");

    name = items.at(0);

    QString imgFilename = items.at(1);
    currentFileName = imgFilename;
    QFileInfo imgInfo(imgFilename);
    imgDirectory = imgInfo.absoluteDir();
    currentFileIt = fileNames.begin();

    QString txtPath = items.at(2);
    hasTextDirectory = !txtPath.isEmpty();
    if (hasTextDirectory)
    {
        QFileInfo txtInfo(txtPath);
        txtDirectory = txtInfo.absoluteDir();
    }
}

QString Session::GetImgDirectoryPath()
{
    return imgDirectory.absolutePath();
}

QString Session::GetName()
{
    return name;
}

QString Session::GetCurrentFileName()
{
    return currentFileName;
}

QString Session::GetDescriptor()
{
    QString descriptor = name + ";" + currentFileName + ";";
    if (hasTextDirectory)
    {
        descriptor += txtDirectory.absolutePath();
    }
    return descriptor;
}

void Session::LoadFiles()
{
    QFileInfoList fileInfos = imgDirectory.entryInfoList(QDir::Files);
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
