#include "authorization.h"
#include "appwindow.h"
#include "registration.h"
#include "ui_authorization.h"

authorization::authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::authorization){
    ui->setupUi(this);

        QSqlDatabase mydb;
        mydb = QSqlDatabase::addDatabase("QMYSQL");
        mydb.setHostName("localhost");
        mydb.setUserName("root");
        mydb.setPort(3306);
        mydb.setPassword("0000");
        mydb.setDatabaseName("carrentdb");
        if (!mydb.open()){ui->infoDB->setText("000");}
        else{ui->infoDB->setText("111");}
}

authorization::~authorization()
{
    delete ui;
}

void authorization::on_CreateBtn_clicked(){
    hide();
    Registration RegForm;
    RegForm.setModal(true);
    RegForm.exec();
}


void authorization::on_SingInBtn_clicked(){
    QString username, password;
       username = ui->UserInput->text(); // get the login name from the login edit field
       password = ui->PasswdInput->text(); // get the password from the password edit field
       QSqlQuery query;
       query.prepare("SELECT id FROM usersdata WHERE username = :username AND password = :password");
       query.bindValue(":username", username); // bind the login name to the query
       query.bindValue(":password", password); // bind the password to the query
       if (!query.exec()) {
           qFatal("Query failed: %s", qPrintable(query.lastError().text()));
       }
       if (query.next()) {
           int userId = query.value(0).toInt();
           qDebug("User logged in successfully. User ID: %d", userId);

           hide();
           AppWindow AppWind;
           AppWind.setModal(true);
           AppWind.exec();
       } else {
           ui->ErrorText->setText("Invalid username or password");
       }

}

