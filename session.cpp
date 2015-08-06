#include <QFileInfo>

#include "session.h"

Session::Session(QString filename)
    : currentFile(filename)
{

}

QString Session::GetAsString()
{
    return currentFile.fileName();
}
