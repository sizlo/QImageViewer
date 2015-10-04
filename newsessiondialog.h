#ifndef NEWSESSIONDIALOG_H
#define NEWSESSIONDIALOG_H

#include <QDialog>

namespace Ui {
class NewSessionDialog;
}

class NewSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewSessionDialog(QWidget *parent = 0);
    ~NewSessionDialog();

    QString GetSessionDescriptor();

public slots:
    void ButtonImgPathPushed();
    void ButtonTxtPathPushed();
    void ButtonAddPushed();
    void ButtonCancelPushed();

private:
    bool NameIsValid();
    bool ImgPathIsValid();
    bool TxtPathIsValid();

    Ui::NewSessionDialog *ui;
};

#endif // NEWSESSIONDIALOG_H
