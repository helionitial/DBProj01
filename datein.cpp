#include "datein.h"
#include "ui_datein.h"
#include <QSqlQuery>
#include <QDebug>

DateIn::DateIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DateIn)
{
    ui->setupUi(this);
}

DateIn::~DateIn()
{
    delete ui;
}
HomeWindow *DateIn::getParent() const
{
    return parent;
}

void DateIn::setParent(HomeWindow *value)
{
    parent = value;
}


/* send back date, update database*/
void DateIn::on_buttonBox_accepted()
{
    qDebug() << book_id << branch_id;
    this->date_in = ui->dateEdit->date();
    QSqlQuery update;

    // update date in
    qDebug() << update.exec("UPDATE book_loans SET date_int='" + date_in.toString("yyyy-MM-dd") +
                 "' WHERE book_id='" + book_id + "' AND branch_id=" + branch_id + ";");
    update.exec("update book_available set No_of_avail_copies = No_of_avail_copies-1 where book_id='" + book_id + "' and branch_id=" + branch_id + ";");
    update.exec("update book_copies set no_of_copies = no_of_copies+1 where book_id='" + book_id + "' and branch_id=" + branch_id + ";");
    // set available number correct:




}
QString DateIn::getBranch_id() const
{
    return branch_id;
}

void DateIn::setBranch_id(const QString &value)
{
    branch_id = value;
}

QString DateIn::getBook_id() const
{
    return book_id;
}

void DateIn::setBook_id(const QString &value)
{
    book_id = value;
}


QDate DateIn::getDate_in() const
{
    return date_in;
}

void DateIn::setDate_in(const QDate &value)
{
    date_in = value;
}

