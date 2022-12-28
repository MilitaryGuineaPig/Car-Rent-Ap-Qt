#include "registration.h"
#include "authorization.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration){
    ui->setupUi(this);

}

Registration::~Registration(){delete ui;}

void Registration::on_RegistrBtn_clicked(){
       QString firstname, lastname, email, username, password, confpassword;
       static int id;
       firstname = ui->FirstNameInput->text();
       lastname = ui->LastNameInput->text();
       email = ui->EmailInput->text();
       username = ui->UserInput->text();
       password = ui->PasswdInput->text();
       confpassword = ui->ConfirmPasswdInput->text();

       QSqlQuery query;
       query.prepare("SELECT * FROM usersdata WHERE username = :username");
       query.bindValue(":username", username);
       if (query.exec()) {
         if (query.first()) {
           ui->ErrorText->setText("Change the username!");
         }
         else {
             if (!password.isEmpty() && !firstname.isEmpty() && !lastname.isEmpty() && !email.isEmpty() && !username.isEmpty() && !password.isEmpty()){
                 if(password == confpassword){
                     query.prepare("SELECT id FROM usersdata WHERE id = (SELECT MAX(id) FROM usersdata)");
                     if (query.exec()) {
                        if (query.first()) { id = query.value(0).toInt();}
                        }
                     else { id = 0; }

                     query.prepare("INSERT INTO usersdata (id, firstname, lastname, email, username, password) VALUES (:id, :firstname, :lastname, :email, :username, :password)");
                     query.bindValue(":id", ++id); // Assuming that you have a variable called "id" that contains the value for the id column
                     query.bindValue(":firstname", firstname);
                     query.bindValue(":lastname", lastname);
                     query.bindValue(":email", email);
                     query.bindValue(":username", username);
                     query.bindValue(":password", password);
                     if (!query.exec()) {
                       ui->ErrorText->setText("Failed, try again!");
                     }
                     else{
                       ui->ErrorText->setText("Successful!");
                       hide();
                       authorization AppWind;
                       AppWind.setModal(true);
                       AppWind.exec();
                     }
                 }
                 else{
                     ui->ErrorText->setText("Check the passwords!");
                 }
             }
             else{
                  ui->ErrorText->setText("Fill all rows!");
             }
         }
       }
       else {
          ui->ErrorText->setText("Something went wrong with database!");
       }

}
