#ifndef EDITCELL_H
#define EDITCELL_H

#include <QDialog>
#include "homewindow.h"

namespace Ui {
class EditCell;
}

class EditCell : public QDialog
{
    Q_OBJECT

public:
    explicit EditCell(QWidget *parent = 0);
    ~EditCell();

    void init();
    int getColumn() const;
    void setColumn(int value);

    QString getData() const;
    void setData(const QString &value);

    int getLoan_id() const;
    void setLoan_id(int value);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EditCell *ui;
    HomeWindow *parent;
    int column;
    QString data;
    int loan_id;
};

#endif // EDITCELL_H
