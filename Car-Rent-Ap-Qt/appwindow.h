#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {class AppWindow;}

class AppWindow : public QDialog{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);
    ~AppWindow();

private slots:
    void on_ProfileBtn_clicked();

    void on_RentCarBtn_clicked();

    void on_ReturnCarBtn_clicked();

    void on_PriceBtn_clicked();

    void on_LogoutBtn_clicked();

    void on_RentBtn_clicked();

    void on_ReturnBtn_clicked();

    void refreshTableViews();

private:
    Ui::AppWindow *ui;
};

#endif // APPWINDOW_H
