#include "dbconnector.h"
#include "ui_dbconnector.h"

DBConnector::DBConnector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBConnector)
{
    ui->setupUi(this);
}

DBConnector::~DBConnector()
{
    delete ui;
}

void DBConnector::on_buttonBox_accepted()
{
    parent->setupDB(ui->lineEdit->text(),
                    ui->lineEdit_2->text(),
                    ui->lineEdit_3->text(),
                    ui->lineEdit_4->text());
    parent->connectDB();
}
HomeWindow *DBConnector::getParent() const
{
    return parent;
}

void DBConnector::setParent(HomeWindow *value)
{
    parent = value;
}

