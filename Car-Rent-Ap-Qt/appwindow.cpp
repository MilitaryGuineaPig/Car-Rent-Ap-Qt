#include "appwindow.h"
#include "ui_appwindow.h"
#include "authorization.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QTimer>
#include <QLabel>

static int noteid = 0;

AppWindow::AppWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppWindow){

    ui->setupUi(this);
    //Profile part
    QSqlQuery queryProfile;
      queryProfile.prepare("SELECT firstname, lastname, email, phone, username, password FROM usersdata WHERE id = :id");
      queryProfile.bindValue(":id", getId()); // Replace 1 with the desired user ID
      if (queryProfile.exec()) {
        while (queryProfile.next()) {
        QString firstname = queryProfile.value(0).toString();
        QString lastname = queryProfile.value(1).toString();
        QString email = queryProfile.value(2).toString();
        QString phone = queryProfile.value(3).toString();
        QString username = queryProfile.value(4).toString();
        QString password = queryProfile.value(5).toString();

        ui->FirstNameText_2->setText(firstname);
        ui->LastNameText_2->setText(lastname);
        ui->EmailText_2->setText(email);
        ui->PhoneText_2->setText(phone);
        ui->UsernameText_2->setText(username);
        ui->PasswordText_2->setText(password);
      }
      } else {
        ui->FirstNameText_2->setText("Error executing query");
      }
    //Rent A Car Part
    QSqlTableModel *tableModelRent;
      tableModelRent = new QSqlTableModel();
      tableModelRent->setTable("cars");
      tableModelRent->setFilter("rentedbyid < 1");
     int row = tableModelRent->rowCount();
      tableModelRent->insertRow(row);
      tableModelRent->setData(tableModelRent->index(row, 0), "$");
      emit tableModelRent->dataChanged(tableModelRent->index(row, 0), tableModelRent->index(row, 0));
      tableModelRent->select();
      ui->RentCarDbView->setModel(tableModelRent);
      ui->RentCarDbView->hideColumn(5);
      ui->RentCarDbView->setColumnWidth(0, 50);
      ui->RentCarDbView->setColumnWidth(1, 150);
      ui->RentCarDbView->setColumnWidth(2, 150);
      ui->RentCarDbView->setColumnWidth(3, 80);
      ui->RentCarDbView->setColumnWidth(4, 100);
      ui->RentCarDbView->show();
    //Return A Car Part
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
    //Price Part
    QSqlTableModel *tableModelPrice;
      tableModelPrice = new QSqlTableModel();
      tableModelPrice->setTable("rentdatedb");
        userId = getId();
        QString filterStringg = QString("userid = %1").arg(userId);
        tableModelPrice->setFilter(filterStringg);
      tableModelPrice->select();
      ui->PriceDbView->setModel(tableModelPrice);
      ui->PriceDbView->setColumnWidth(0, 40);
      ui->PriceDbView->hideColumn(1);
      ui->PriceDbView->hideColumn(2);
      ui->PriceDbView->setColumnWidth(3, 90);
      ui->PriceDbView->setColumnWidth(4, 90);
      ui->PriceDbView->setColumnWidth(5, 120);
      ui->PriceDbView->setColumnWidth(6, 130);
      ui->PriceDbView->setColumnWidth(7, 70);
      ui->PriceDbView->hideColumn(8);
      ui->PriceDbView->show();


    //The end
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
    QSqlTableModel *tableModelPrice = static_cast<QSqlTableModel*>(ui->PriceDbView->model());
    tableModelPrice->select();
    tableModelPrice->layoutChanged();
}

//Rent A Car Part
void AppWindow::on_RentBtn_clicked() {
    QString carsId = ui->CarsIdInput->text();
    // Check if the car is already rented
    QSqlQuery query;
     query.prepare("SELECT rentedbyid, brand, model FROM cars WHERE id = :id");
     query.bindValue(":id", carsId);
    if (!query.exec()) {
        ui->ErrorText->setText("Error opening!");
        QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
        return;
    }
    if (!query.next()) {
        ui->ErrorText->setText("Car not found!");
        QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
        return;
    }
    int rentedbyid = query.value(0).toInt();
    QString brand= query.value(1).toString();
    QString model= query.value(2).toString();
    if (rentedbyid != 0) {
        ui->ErrorText->setText("Car already rented!");
        QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
        return;
    }
    // Rent the car
    QSqlQuery queryRent;
     queryRent.prepare("UPDATE cars SET rentedbyid = :rentedbyid WHERE id = :carsId");
     queryRent.bindValue(":rentedbyid", getId());
     queryRent.bindValue(":carsId", carsId);
     if (!queryRent.exec()) {
        ui->ErrorText->setText("Error inserting!");
        QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
        return;
     }
    //Fill rentdb with info
    QSqlQuery queryRentDate;
      queryRentDate.prepare("SELECT idnote FROM rentdatedb WHERE idnote = (SELECT MAX(idnote) FROM rentdatedb)");
      if (queryRentDate.exec()) {
         if (queryRentDate.first()) { noteid = queryRentDate.value(0).toInt();}
         }
      else { noteid = 0;}

      QDate currdate = QDate::currentDate();
        queryRentDate.prepare("INSERT INTO rentdatedb (idnote, userid, carid, daterent, brand, model) VALUES (:idnote, :userid, :carid, :daterent, :brand, :model)");
        queryRentDate.bindValue(":idnote", ++noteid);
        queryRentDate.bindValue(":userid", getId());
        queryRentDate.bindValue(":carid", carsId);
        queryRentDate.bindValue(":daterent", currdate);
        queryRentDate.bindValue(":brand", brand);
        queryRentDate.bindValue(":model", model);
        if (!queryRentDate.exec()) {
          ui->ErrorText->setText("Error inserting!");
          QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
          return;
         }

      ui->ErrorText->setText("Rented!");
      ui->CarsIdInput->setText("");
      QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });

    // Refresh the table views
    refreshTableViews();
}

//Return A Car Part
void AppWindow::on_ReturnBtn_clicked() {
    QString carId = ui->CarsIdReturnInput->text();

    // Check if the car is already rented
    QSqlQuery queryCheck;
      queryCheck.prepare("SELECT rentedbyid FROM cars WHERE id = :id");
      queryCheck.bindValue(":id", carId);
     if (!queryCheck.exec()) {
        ui->ErrorText_2->setText("Error query check!");
        return;
      }
     if (!queryCheck.next()) {
        ui->ErrorText_2->setText("Car not found check!");
        return;
      }
     int rentedbyid = queryCheck.value(0).toInt();
     if (rentedbyid != getId()) {
        ui->ErrorText_2->setText("Car is not rented by you!");
        return;
      }


    //add return date
    QSqlQuery queryAddReturnDate;
      queryAddReturnDate.prepare("UPDATE rentdatedb SET datereturn = CURRENT_DATE WHERE (userid, carid) = (:userid, :carid)");
      queryAddReturnDate.bindValue(":userid", getId());
      queryAddReturnDate.bindValue(":carid", carId);
     if (!queryAddReturnDate.exec()) {
          ui->ErrorText_2->setText("Error additing return date!");
          return;
        }


    // Get the return date from the rentdatedb table and add price

    //select dates
    QSqlQuery queryDate;
      queryDate.prepare("SELECT daterent, datereturn FROM rentdatedb WHERE userid = :userid AND carid = :carid");
      queryDate.bindValue(":userid", getId());
      queryDate.bindValue(":carid", carId);
    if (!queryDate.exec()) {
        // There was an error executing the query
        ui->ErrorText_2->setText("Error selecting dates");
        return;
    }
    if (!queryDate.next()) {
        // There are no records in the result set
        ui->ErrorText_2->setText("No records found");
        return;
    }
    // The query was successful and there are records in the result set
    QDate rentDate = queryDate.value(0).toDate();
    QDate returnDate = queryDate.value(1).toDate();
    int days = rentDate.daysTo(returnDate) + 1;

    QSqlQuery queryCarPrice;
    queryCarPrice.prepare("SELECT price FROM cars WHERE id = :id");
    queryCarPrice.bindValue(":id", carId);
    if (!queryCarPrice.exec()) {
        ui->ErrorText_2->setText("Error selecting car price");
        return;
    }
    if (!queryCarPrice.next()) {
        ui->ErrorText_2->setText("Car not found");
        return;
    }
    int rentpriceday = queryCarPrice.value(0).toInt();
    int price = days * rentpriceday;

    QSqlQuery queryUpdatePriceAndDays;
    queryUpdatePriceAndDays.prepare("UPDATE rentdatedb SET price = :price, days = :days WHERE userid = :userid AND carid = :carid AND price = '0' ");
    queryUpdatePriceAndDays.bindValue(":price", price);
    queryUpdatePriceAndDays.bindValue(":days", days);
    queryUpdatePriceAndDays.bindValue(":userid", getId());
    queryUpdatePriceAndDays.bindValue(":carid", carId);
    if (!queryUpdatePriceAndDays.exec()) {
        ui->ErrorText_2->setText("Error updating price and days");
        return;
    }

    // Update the car's rentedbyid to 0
    QSqlQuery queryReturn;
      queryReturn.prepare("UPDATE cars SET rentedbyid = :rentedbyid WHERE id = :id");
      queryReturn.bindValue(":rentedbyid", 0);
      queryReturn.bindValue(":id", carId);
     if (!queryReturn.exec()) {
       ui->ErrorText_2->setText("Error returning car");
       return;
     }


    //finish
    ui->ErrorText_2->setText("Returned!");
    ui->CarsIdReturnInput->setText("");
    QTimer::singleShot(2000, [this] { ui->ErrorText_2->setText(""); });

    // price text
    QSqlQuery queryPriceCounter;
    queryPriceCounter.prepare("SELECT SUM(price) FROM rentdatedb WHERE userid = :userid AND price > 1");
    queryPriceCounter.bindValue(":userid", getId());
    if (!queryPriceCounter.exec()) {
        ui->PriceShow->setText("Error opening price!");
        QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
        return;
    }
    if (!queryPriceCounter.next()) {
        ui->PriceShow->setText("Price not found!");
        QTimer::singleShot(2000, [this] { ui->ErrorText->setText(""); });
        return;
    }
    int priceSum = queryPriceCounter.value(0).toInt();
    QString sum = QString::number(priceSum);
    ui->PriceShow->setText(sum + "$");

    // Refresh the table views
    refreshTableViews();
}

