#ifndef DATEIN_H
#define DATEIN_H

#include <QDialog>
#include "homewindow.h"
namespace Ui {
class DateIn;
}

class DateIn : public QDialog
{
    Q_OBJECT

public:
    explicit DateIn(QWidget *parent = 0);
    ~DateIn();

    HomeWindow *getParent() const;
    void setParent(HomeWindow *value);

    QDate getDate_in() const;
    void setDate_in(const QDate &value);

    QString getBook_id() const;
    void setBook_id(const QString &value);

    QString getBranch_id() const;
    void setBranch_id(const QString &value);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DateIn *ui;
    HomeWindow *parent;
    QDate date_in;
    QString book_id;
    QString branch_id;
};

#endif // DATEIN_H
