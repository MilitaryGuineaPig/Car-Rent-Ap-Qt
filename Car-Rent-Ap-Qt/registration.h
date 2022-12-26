#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>

#include <QtSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_RegistrBtn_clicked();

private:
    Ui::Registration *ui;
};

#endif // REGISTRATION_H
