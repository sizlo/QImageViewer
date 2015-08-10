#ifndef SESSION_H
#define SESSION_H

#include <QDir>

class Session
{
public:
    Session(QString filename);

    QString GetAsString();
    QString GetCurrentFileName();

    void LoadFiles();
    void NextFile();
    void PreviousFile();

private:
    QDir directory;
    QString currentFileName;
    QStringList fileNames;
    QStringList::iterator currentFileIt;
};

#endif // SESSION_H
