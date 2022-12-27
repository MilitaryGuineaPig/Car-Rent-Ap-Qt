#include "appwindow.h"
#include "ui_appwindow.h"
#include "authorization.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QStyledItemDelegate>

AppWindow::AppWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppWindow){

    ui->setupUi(this);

    //Profile part
    QSqlQuery queryProfile;
    queryProfile.prepare("SELECT firstname, lastname, email, username, password FROM usersdata WHERE id = :id");
    queryProfile.bindValue(":id", getId()); // Replace 1 with the desired user ID
    if (queryProfile.exec()) {
      while (queryProfile.next()) {
        QString firstname = queryProfile.value(0).toString();
        QString lastname = queryProfile.value(1).toString();
        QString email = queryProfile.value(2).toString();
        QString username = queryProfile.value(3).toString();
        QString password = queryProfile.value(4).toString();

        ui->FirstNameText_2->setText(firstname);
        ui->LastNameText_2->setText(lastname);
        ui->EmailText_2->setText(email);
        ui->UsernameText_2->setText(username);
        ui->PasswordText_2->setText(password);
      }
    } else {
      ui->FirstNameText_2->setText("Error executing query");
    }




    //Rent A Car Part
    QSqlTableModel *tableModel;
    tableModel = new QSqlTableModel();
    tableModel->setTable("cars");
    tableModel->select();
    ui->RentCarDbView->setModel(tableModel);
    ui->RentCarDbView->show();
    //Return A Car Part
}

AppWindow::~AppWindow(){ delete ui;}

void AppWindow::on_ProfileBtn_clicked(){ ui->stackedWidget->setCurrentIndex(0); }
void AppWindow::on_RentCarBtn_clicked(){ ui->stackedWidget->setCurrentIndex(1); }
void AppWindow::on_ReturnCarBtn_clicked(){ ui->stackedWidget->setCurrentIndex(2); }
void AppWindow::on_PriceBtn_clicked(){ ui->stackedWidget->setCurrentIndex(3); }
void AppWindow::on_LogoutBtn_clicked(){
    setId(0);
    hide();
    authorization AppWind;
    AppWind.setModal(true);
    AppWind.exec();
}

