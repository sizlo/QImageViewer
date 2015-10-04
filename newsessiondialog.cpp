#include "newsessiondialog.h"
#include "ui_newsessiondialog.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qdebug.h"

NewSessionDialog::NewSessionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSessionDialog)
{
    ui->setupUi(this);

    setWindowTitle("QImageViewer | New Session");

    QObject::connect(ui->imgPathButton,     SIGNAL(clicked()), this, SLOT(ButtonImgPathPushed()));
    QObject::connect(ui->txtPathButton,     SIGNAL(clicked()), this, SLOT(ButtonTxtPathPushed()));
    QObject::connect(ui->addSessionButton,  SIGNAL(clicked()), this, SLOT(ButtonAddPushed()));
    QObject::connect(ui->cancelButton,      SIGNAL(clicked()), this, SLOT(ButtonCancelPushed()));
}

NewSessionDialog::~NewSessionDialog()
{
    delete ui;
}

QString NewSessionDialog::GetSessionDescriptor()
{
    QString name = ui->nameLineEdit->text();
    QString imgPath = ui->imgPathLineEdit->text();
    QString txtPath = ui->txtPathLineEdit->text();

    QString descriptor = name + ";" + imgPath + ";" + txtPath;
    return descriptor;
}

void NewSessionDialog::ButtonImgPathPushed()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
    {
        QString filename = dialog.selectedFiles().at(0);
        ui->imgPathLineEdit->setText(filename);
    }
}

void NewSessionDialog::ButtonTxtPathPushed()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec())
    {
        QString filename = dialog.selectedFiles().at(0);
        ui->txtPathLineEdit->setText(filename);
    }
}

void NewSessionDialog::ButtonAddPushed()
{
    if (!NameIsValid() || !ImgPathIsValid() || !TxtPathIsValid())
    {
        return;
    }
    accept();
}

void NewSessionDialog::ButtonCancelPushed()
{
    reject();
}

bool NewSessionDialog::NameIsValid()
{
    QString name = ui->nameLineEdit->text();
    if (name.isEmpty())
    {
        QMessageBox m;
        m.setText("Name is required");
        m.exec();
        return false;
    }
    if (name.contains(';'))
    {
        QMessageBox m;
        m.setText("Name cannot contain ;");
        m.exec();
        return false;
    }
    return true;
}

bool NewSessionDialog::ImgPathIsValid()
{
    QString imgPath = ui->imgPathLineEdit->text();
    if (imgPath.isEmpty())
    {
        QMessageBox m;
        m.setText("Image Path is required");
        m.exec();
        return false;
    }
    QFileInfo info(imgPath);
    if (!(info.exists() && info.isFile()))
    {
        QMessageBox m;
        m.setText("Image Path must be an existing file");
        m.exec();
        return false;
    }
    return true;
}

bool NewSessionDialog::TxtPathIsValid()
{
    QString txtPath = ui->txtPathLineEdit->text();
    if (txtPath.isEmpty())
    {
        // txtPath is not required
        return true;
    }
    QFileInfo info(txtPath);
    if (!(info.exists() && info.isDir()))
    {
        QMessageBox m;
        m.setText("Text Path must be an existing directory");
        m.exec();
        return false;
    }
    return true;
}
