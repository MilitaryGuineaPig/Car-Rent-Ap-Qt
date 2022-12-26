#include "appwindow.h"
#include "ui_appwindow.h"

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
        ui->DbView->setModel(tableModel);
        ui->DbView->show();
}

AppWindow::~AppWindow(){ delete ui;}
