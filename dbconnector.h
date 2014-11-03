#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <QDialog>
#include "homewindow.h"

namespace Ui {
class DBConnector;
}

class DBConnector : public QDialog
{
    Q_OBJECT

public:
    explicit DBConnector(QWidget *parent = 0);
    ~DBConnector();

    HomeWindow *getParent() const;
    void setParent(HomeWindow *value);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DBConnector *ui;
    HomeWindow *parent;
};

#endif // DBCONNECTOR_H
