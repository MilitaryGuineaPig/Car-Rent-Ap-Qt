#include "appwindow.h"
#include "ui_appwindow.h"
#include "authorization.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QStyledItemDelegate>

AppWindow::AppWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppWindow)
{
    ui->setupUi(this);

    //ui->stackedWidget->setCurrentIndex(0);
    QSqlTableModel *tableModel;
        tableModel = new QSqlTableModel();
        tableModel->setTable("cars");
        tableModel->select();
        ui->RentCarDbView->setModel(tableModel);
        ui->RentCarDbView->show();
}

AppWindow::~AppWindow(){ delete ui;}

void AppWindow::on_ProfileBtn_clicked(){ ui->stackedWidget->setCurrentIndex(0); }
void AppWindow::on_RentCarBtn_clicked(){ ui->stackedWidget->setCurrentIndex(1); }
void AppWindow::on_ReturnCarBtn_clicked(){ ui->stackedWidget->setCurrentIndex(2); }
void AppWindow::on_PriceBtn_clicked(){ ui->stackedWidget->setCurrentIndex(3); }
void AppWindow::on_LogoutBtn_clicked(){
    hide();
    authorization AppWind;
    AppWind.setModal(true);
    AppWind.exec();
}

