#include <QFileInfo>
#include <QDebug>

#include "session.h"
#include "mainwindow.h"

// Function used to sort strings where whole numbers present in the string are compared
// and not seperate digits
bool lessThanObeyingNumericOrder(const QString lhs, const QString rhs)
{
    int lhsLength = lhs.length();
    int rhsLength = rhs.length();

    int currentLhsCharPos = 0;
    int currentRhsCharPos = 0;
    while (currentLhsCharPos < lhsLength && currentRhsCharPos < rhsLength)
    {
        QChar lhsChar = lhs.at(currentLhsCharPos);
        QChar rhsChar = rhs.at(currentRhsCharPos);

        // If both characters are digits find the full number and compare the two
        if (lhsChar.isDigit() && rhsChar.isDigit())
        {
            QString lhsNum = lhsChar;
            while (lhs.at(currentLhsCharPos + 1).isDigit())
            {
                currentLhsCharPos++;
                lhsNum += lhs.at(currentLhsCharPos);
            }
            QString rhsNum = rhsChar;
            while (rhs.at(currentRhsCharPos + 1).isDigit())
            {
                currentRhsCharPos++;
                rhsNum += rhs.at(currentRhsCharPos);
            }
            if (lhsNum.toInt() != rhsNum.toInt())
            {
                return lhsNum.toInt() < rhsNum.toInt();
            }
        }
        else
        {
            if (lhsChar != rhsChar)
            {
                return lhsChar < rhsChar;
            }
        }

        currentLhsCharPos++;
        currentRhsCharPos++;
    }

    return false;
}

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
        txtDirectory = txtInfo.absoluteFilePath();
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

QString Session::GetCurrentHoverText()
{
    QFileInfo imageInfo(currentFileName);
    QString baseName = imageInfo.baseName();
    QString textPath = txtDirectory.absolutePath() + QDir::separator() + baseName + ".txt";
    QFileInfo textInfo(textPath);
    if (textInfo.exists() && textInfo.isFile())
    {
        QFile textFile(textPath);
        if (textFile.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&textFile);
            QString hoverText = stream.readAll();
            return hoverText;
        }
    }

    return "";
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
    // Make sure we empty the list so we don't just populate it with duplicates
    fileNames.clear();

    QFileInfoList fileInfos = imgDirectory.entryInfoList(QDir::Files);
    for (auto info: fileInfos)
    {
        fileNames.push_back(info.absoluteFilePath());
    }
    qSort(fileNames.begin(), fileNames.end(), lessThanObeyingNumericOrder);

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

void Session::FirstFile()
{
    currentFileIt = fileNames.begin();
    currentFileName = *currentFileIt;

    MainWindow::Get()->PopulateSessionList();
}

void Session::LastFile()
{
    currentFileIt = fileNames.end();
    currentFileIt--;
    currentFileName = *currentFileIt;

    MainWindow::Get()->PopulateSessionList();
}
