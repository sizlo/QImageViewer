#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "session.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ButtonOpenSessionPushed();
    void ButtonDeleteSessionPushed();
    void ButtonNewSessionPushed();

private:
    void LoadSettings();
    void SaveSettings();
    QStringList GetSessionNames();
    void PopulateSessionList();
    Session * GetCurrentSessionSelection();

    Ui::MainWindow *ui;
    std::vector<Session *> sessions;
    QString settingsFilePath;
};

#endif // MAINWINDOW_H
