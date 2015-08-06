#ifndef SESSION_H
#define SESSION_H

#include <QFile>

class Session
{
public:
    Session(QString filename);

    QString GetAsString();

private:
    QFile currentFile;
};

#endif // SESSION_H
