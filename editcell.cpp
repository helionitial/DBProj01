#include "editcell.h"
#include "ui_editcell.h"
#include <QSqlQuery>

EditCell::EditCell(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCell)
{
    ui->setupUi(this);
}
void EditCell::init()
{
    ui->lineEdit->setText(data);
}

EditCell::~EditCell()
{
    delete ui;
}
int EditCell::getColumn() const
{
    return column;
}

void EditCell::setColumn(int value)
{
    column = value;
}
QString EditCell::getData() const
{
    return data;
}

void EditCell::setData(const QString &value)
{
    data = value;
}

/*
 * update the database
 */

void EditCell::on_buttonBox_accepted()
{
    QString input = ui->lineEdit->text();
    QSqlQuery q;
    if(column == 1) //  update fines
    {
        q.exec("update fines set fine_amt='" + input +
               "' where loan_id=" + QString::number(loan_id)
                    );
    }else if(column == 2)   // update paid or not
    {
        q.exec("update fines set paid=" + input +
               " where loan_id=" + QString::number(loan_id)
                    );

    }else{
        // nothing to do
    }
}
int EditCell::getLoan_id() const
{
    return loan_id;
}

void EditCell::setLoan_id(int value)
{
    loan_id = value;
}

