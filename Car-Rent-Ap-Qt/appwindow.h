#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class AppWindow;
}

class AppWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);
    ~AppWindow();

private:
    Ui::AppWindow *ui;
};

#endif // APPWINDOW_H
