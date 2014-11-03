#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QDate>
#include <QMainWindow>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();

    void setupDB(QString s1, QString s2, QString s3, QString s4);
    void connectDB();
    QDate getDateIn() const;
    void setDateIn(const QDate &value);

private slots:
    void on_actionConnect_triggered();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_2_doubleClicked(const QModelIndex &index);

    void on_actionRefresh_triggered();

    void on_actionFines_Management_triggered();

    void on_pushButton_7_clicked();

    void on_actionInitial_Fines_triggered();

    void on_pushButton_8_clicked();

    void on_tableView_3_doubleClicked(const QModelIndex &index);

    void on_actionSearch_triggered();

    void on_actionBorrower_Management_triggered();

    void on_actionCheck_Out_triggered();

    void on_actionCheck_In_triggered();

    void on_pushButton_6_clicked();

private:
    Ui::HomeWindow *ui;
    QString dbhost;
    QString dbname;
    QString username;
    QString password;
    QDate dateIn;

    void showAllBooks();
    void init();

};

#endif // HOMEWINDOW_H
