#ifndef ADDNEWDIALOG_H
#define ADDNEWDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QtXml>
#include <QString>
#include "global.h"
#include "settings.h"

class AddNewDialog : public QDialog
{
    Q_OBJECT
public:
    AddNewDialog();
    QString getName();
    QString getProfile();
    QComboBox *getNamesComboBox();

private slots:
    void namesComboBoxChanged(QString newName);

private:
    QGridLayout *layout;    
    QDialogButtonBox *buttonBox;
    QComboBox *namesComboBox;
    QComboBox *profileComboBox;

};

#endif // ADDNEWDIALOG_H
