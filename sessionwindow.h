#ifndef SESSIONWINDOW_H
#define SESSIONWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "session.h"

namespace Ui {
class SessionWindow;
}

class SessionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SessionWindow(Session *s, QWidget *parent = 0);
    ~SessionWindow();

    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    Session * GetSession();

public slots:
    void ButtonNextPushed();
    void ButtonPreviousPushed();
    void ButtonFirstPushed();
    void ButtonLastPushed();

private:
    void ShowCurrentImage();

    Ui::SessionWindow *ui;
    Session *session;
};

#endif // SESSIONWINDOW_H
