#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "session.h"
#include "sessionwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

    void PopulateSessionList();
    void RemoveActiveSessionWindow(SessionWindow *w);

    static MainWindow * Get();

public slots:
    void ButtonOpenSessionPushed();
    void ButtonDeleteSessionPushed();
    void ButtonNewSessionPushed();

private:
    void LoadSettings();
    void SaveSettings();
    QStringList GetSessionNames();
    Session * GetCurrentSessionSelection();

    Ui::MainWindow *ui;
    std::vector<Session *> sessions;
    QString settingsFilePath;
    std::list<SessionWindow *> activeSessionWindows;

    static MainWindow *instance;
};

#endif // MAINWINDOW_H
