#include "appwindow.h"
#include "ui_appwindow.h"
#include "authorization.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QTimer>
#include <QLabel>

AppWindow::AppWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppWindow){

    ui->setupUi(this);
    //
    //Profile part
    //
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
    //
    //Rent A Car Part
    //
    QSqlTableModel *tableModelRent;
    tableModelRent = new QSqlTableModel();
    tableModelRent->setTable("cars");
    tableModelRent->setFilter("rentedbyid < 1");
    tableModelRent->select();
    ui->RentCarDbView->setModel(tableModelRent);
    ui->RentCarDbView->hideColumn(5);
    ui->RentCarDbView->setColumnWidth(0, 50);
    ui->RentCarDbView->setColumnWidth(1, 150);
    ui->RentCarDbView->setColumnWidth(2, 150);
    ui->RentCarDbView->setColumnWidth(3, 80);
    ui->RentCarDbView->setColumnWidth(4, 100);
    ui->RentCarDbView->show();
    //
    //Return A Car Part
    //
    QSqlTableModel *tableModelReturn;
        tableModelReturn = new QSqlTableModel();
        tableModelReturn->setTable("cars");
        int userId = getId();
        QString filterString = QString("rentedbyid = %1").arg(userId);
        tableModelReturn->setFilter(filterString);
        tableModelReturn->select();
    ui->ReturnCarDbView->setModel(tableModelReturn);
    ui->ReturnCarDbView->hideColumn(5);
    ui->ReturnCarDbView->setColumnWidth(0, 50);
    ui->ReturnCarDbView->setColumnWidth(1, 150);
    ui->ReturnCarDbView->setColumnWidth(2, 150);
    ui->ReturnCarDbView->setColumnWidth(3, 80);
    ui->ReturnCarDbView->setColumnWidth(4, 100);
    ui->ReturnCarDbView->show();
    //
    //Price Part
    //
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

void AppWindow::refreshTableViews() {
    QSqlTableModel *tableModelRent = static_cast<QSqlTableModel*>(ui->RentCarDbView->model());
    tableModelRent->select();
    tableModelRent->layoutChanged();
    QSqlTableModel *tableModelReturn = static_cast<QSqlTableModel*>(ui->ReturnCarDbView->model());
    tableModelReturn->select();
    tableModelReturn->layoutChanged();
}

//Rent A Car Part
void AppWindow::on_RentBtn_clicked() {
    QString carsId = ui->CarsIdInput->text();
    // Check if the car is already rented
    QSqlQuery query;
     query.prepare("SELECT rentedbyid FROM cars WHERE id = :id");
     query.bindValue(":id", carsId);
    if (!query.exec()) {
        ui->ErrorText->setText("Error opening!");
        return;
    }
    if (!query.next()) {
        ui->ErrorText->setText("Car not found!");
        return;
    }
    int rentedbyid = query.value(0).toInt();
    if (rentedbyid != 0) {
        ui->ErrorText->setText("Car already rented!");
        return;
    }
    // Rent the car
    QSqlQuery queryRent;
     queryRent.prepare("UPDATE cars SET rentedbyid = :rentedbyid WHERE id = :carsId");
     queryRent.bindValue(":rentedbyid", getId());
     queryRent.bindValue(":carsId", carsId);
     if (!queryRent.exec()) {
        ui->ErrorText->setText("Error inserting!");
        return;
     }
    ui->ErrorText->setText("Rented!");
    QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
    // Refresh the table views
    refreshTableViews();
}

//Return A Car Part
void AppWindow::on_ReturnBtn_clicked() {
    QString carsId = ui->CarsIdReturnInput->text();

    QSqlQuery query;
    query.prepare("SELECT rentedbyid FROM cars WHERE id = :id");
    query.bindValue(":id", carsId);

    if (query.exec()) {
        if (query.next()) {
            int rentedbyid = query.value(0).toInt();
            if (rentedbyid == getId()) {
                QSqlQuery updateQuery;
                updateQuery.prepare("UPDATE cars SET rentedbyid = :rentedbyid WHERE id = :id");
                updateQuery.bindValue(":rentedbyid", 0);
                updateQuery.bindValue(":id", carsId);

                if (updateQuery.exec()) {
                    ui->ErrorText_2->setText("Returned!");
                } else {
                    ui->ErrorText_2->setText("Error updating car");
                }
            } else {
                ui->ErrorText_2->setText("Car is not rented by you");
            }
        } else {
            ui->ErrorText_2->setText("Car not found");
        }
    } else {
        ui->ErrorText_2->setText("Error executing query");
    }

    refreshTableViews();
}

