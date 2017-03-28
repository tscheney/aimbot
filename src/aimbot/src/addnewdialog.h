#ifndef ADDNEWDIALOG_H
#define ADDNEWDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QComboBox>
#include <QSettings>
#include "global.h"

class AddNewDialog : public QDialog
{
    Q_OBJECT
public:
    AddNewDialog();

private slots:
    void namesComboBoxChanged(QString newName);

private:
    QGridLayout *layout;
    QComboBox *namesComboBox;
    QComboBox *profileComboBox;

};

#endif // ADDNEWDIALOG_H
