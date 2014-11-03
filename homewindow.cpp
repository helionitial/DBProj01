#include "dbconnector.h"
#include "homewindow.h"
#include "ui_homewindow.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QDate>
#include <QMessageBox>
#include "datein.h"
#include "finecal.h"
#include "editcell.h"

HomeWindow::HomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    init();
}
void HomeWindow::init()
{
    // setup a delegate for fine management panel  no need.
    // hide all panels

    ui->dockWidget_2->hide();
    ui->dockWidget_3->hide();
    ui->dockWidget_4->hide();
    ui->dockWidget_5->hide();
    ui->dockWidget_6->hide();

    ui->tabWidget->setCurrentIndex(0);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_actionConnect_triggered()
{
    DBConnector *dbc = new DBConnector(this);
    dbc->setParent(this);
    dbc->setVisible(true);
    qDebug() <<"Hello!";
}

void HomeWindow::setupDB(QString s1, QString s2, QString s3, QString s4)
{
    dbhost = s1;
    dbname = s2;
    username = s3;
    password = s4;
}

void HomeWindow::connectDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(dbhost);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);
    if(!db.open()) qDebug() <<" Database connection error!";
    else qDebug() <<" Database connected!";


    // show all books at the query tab
    showAllBooks();
}

void HomeWindow::showAllBooks()
{
    QSqlQuery *query = new QSqlQuery();
    query->exec("select b.book_id,b.title,ba.author_name,ba.type,bc.branch_id,bc.no_of_copies+bav.no_of_avail_copies as total, bc.no_of_copies as available \
                from (book b join book_authors ba join book_copies bc)  join book_available bav \
                WHERE  b.book_id=ba.book_id and b.book_id=bc.book_id and b.book_id=bav.book_id and bav.branch_id=bc.branch_id;");

    QSqlQueryModel *model =  new QSqlQueryModel();
    model->setQuery(*query);
    ui->tableView->setModel(model);


}

// provide book title/author name/book id to search
void HomeWindow::on_pushButton_4_clicked()
{
    QString book_id = ui->lineEdit_10->text();
    QString book_title = ui->lineEdit_11->text();
    QString author_name = ui->lineEdit_12->text();

    if(book_id=="" || book_title=="" || author_name=="")
        return;
    for(int i = 0; i < ui->tableView->model()->rowCount(); ++i)
    {
        bool match = false;
        QModelIndex index1 = ui->tableView->model()->index(i, 0, QModelIndex());
        QModelIndex index2 = ui->tableView->model()->index(i, 1, QModelIndex());
        QModelIndex index3 = ui->tableView->model()->index(i, 2, QModelIndex());
        QTableWidgetItem *item1 = new QTableWidgetItem(ui->tableView->model()->data(index1).toString());
        QTableWidgetItem *item2 = new QTableWidgetItem(ui->tableView->model()->data(index2).toString());
        QTableWidgetItem *item3 = new QTableWidgetItem(ui->tableView->model()->data(index3).toString());

        if(item1->text().contains(book_id) && item2->text().contains(book_title) && item3->text().contains(author_name))
        {

            match = true;
        }

        delete item1;
        delete item2;
        delete item3;

        ui->tableView->setRowHidden(i, !match);
    }
}

// global search for tableview
void HomeWindow::on_pushButton_5_clicked()
{
    QString filter = ui->lineEdit_13->text();

    for(int i = 0; i < ui->tableView->model()->rowCount(); ++i)
    {
        bool match = false;
        for(int j = 0; j < ui->tableView->model()->columnCount(); ++j)
        {
            QModelIndex index = ui->tableView->model()->index(i, j, QModelIndex());
            QTableWidgetItem *item = new QTableWidgetItem(ui->tableView->model()->data(index).toString());

            if(item->text().contains(filter))
            {
                match = true;
                break;
            }
            delete item;

        }
        ui->tableView->setRowHidden(i, !match);
    }
}

// create new borrower
void HomeWindow::on_pushButton_3_clicked()
{

    QSqlQuery *query = new QSqlQuery();
    query->exec("select b.book_id,b.title,ba.author_name,ba.type,bc.branch_id,bc.no_of_copies \
                from book b, book_authors ba, book_copies bc \
                where  b.book_id=ba.book_id and b.book_id=bc.book_id");
    QSqlQueryModel *model =  new QSqlQueryModel();
    model->setQuery(*query);
    ui->tableView->setModel(model);

    QString Fname = ui->lineEdit_6->text();
    QString Lname = ui->lineEdit_7->text();
    QString Address = ui->lineEdit_8->text();
    QString Phone = ui->lineEdit_9->text();

    // validate

    QSqlQuery q;
    q.exec("select COUNT(*) from borrower where Fname='" + Fname + "' and Lname='" + Lname +
           "' and Address='" + Address + "';");
    while(q.next()){
        if(q.value(0).toInt() > 0){
            ui->textBrowser->append("The combination of username/address already exist!");
            return;
        }
    }

    if(Fname.isEmpty() || Lname.isEmpty() || Address.isEmpty()){
        ui->textBrowser->append("Fname/Lname/Address cannot be empty!");
        return;
    }
    if(Fname.isNull() || Lname.isNull() || Address.isNull()){
        ui->textBrowser->append("Fname/Lname/Address cannot be null!");
        return;
    }
    /* auto card_no */

    q.exec("select MAX(card_no) from borrower;");

    int currentIndex;
    while(q.next()){
        currentIndex = q.value(0).toInt();
    }

    ui->textBrowser->append("New borrower card_no: " + QString::number(currentIndex + 1));
    q.exec("insert into borrower values ('" + QString::number(currentIndex + 1) + "','" + Fname +
           "','" + Lname + "','" + Address + "','" + Phone + "');");
    ui->textBrowser->append("New borrower added!");

}

// check out books
/*
 * Input: book_id, branch_id, borrower card_no
 *
 */
void HomeWindow::on_pushButton_clicked()
{
    QString book_id = ui->lineEdit->text();
    QString branch_id = ui->lineEdit_2->text();
    QString card_no = ui->lineEdit_3->text();

    QSqlQuery *check2 = new QSqlQuery();
    check2->exec("select bc.no_of_copies \
                 from book b join book_authors ba join book_copies bc join book_available bav \
                 where  b.book_id=ba.book_id and b.book_id=bc.book_id and b.book_id=bav.book_id and bav.branch_id=bc.branch_id and b.book_id='" + book_id + "' and bc.branch_id=" + branch_id + ";");

    while(check2->next()){
        qDebug() << "check2: no of avail copies:" << check2->value(0) ;
        if(check2->value(0).toInt() < 1){
            QMessageBox::warning(this, tr("Book available number less than 1"),
                                            tr("Book available number less than 1."),
                                            QMessageBox::Cancel);
            return;
        }
    }

    QSqlQuery *query = new QSqlQuery();
    query->exec("select MAX(loan_id) from book_loans;");
    int currentIndex;
    while(query->next()){
        currentIndex = query->value(0).toInt();
        qDebug() << currentIndex << "current index was.";
    }
    QSqlQuery *insert = new QSqlQuery();
    QDate date = QDate::currentDate();
    QDate dueDate = date.addDays(14);


    // exception 1: the borrower already has borrowed 3 books.
    QSqlQuery *check1 = new QSqlQuery();
    check1->exec("select COUNT(*) from book_loans where card_no=" + card_no);



    while(check1->next()){
        if(check1->value(0).toInt() > 2){
            QMessageBox::warning(this, tr("Borrower exceeds 3 limit of books!"),
                                            tr("This user already borrows more than 2 books."),
                                            QMessageBox::Cancel);
            return;

        }
    }
    // exception 2: book available num less than 1


    // allow user to borrow:
    /*
     * 1. add a record in book_loans
     * 2. decrease the number of available
     * 3. increase the number of borrowed
     * 4. add a record in fines
     */
    QSqlQuery *update = new QSqlQuery();
    update->exec("update book_available set No_of_avail_copies = No_of_avail_copies+1 where book_id='" + book_id + "' and branch_id=" + branch_id + ";");
    update->exec("update book_copies set no_of_copies = no_of_copies-1 where book_id='" + book_id + "' and branch_id=" + branch_id + ";");
    insert->exec("insert into book_loans values (" + QString::number(currentIndex + 1) + ", '" + book_id + "', " + branch_id + ", '" + card_no + "', '" +
                 date.toString("yyyy-MM-dd") + "', '" + dueDate.toString("yyyy-MM-dd") + "', 'NULL')");
    insert->exec("insert into fines values (" + QString::number(currentIndex + 1) + ", '0.00',0)");

    QMessageBox::warning(this, tr("Done"),
                                    tr("Operation successful!"),
                                    QMessageBox::Ok);

}

/*
 * Check-in
 * 1. load the book_loans into tab2
 * 2. perform a search/filter on that
 * 3. when click on the tuple, ask to enter a date-in
 *
 */

void HomeWindow::on_pushButton_2_clicked()
{
    QString filter = ui->lineEdit_4->text();

    // load book_loans into tab2
    QSqlQuery *query = new QSqlQuery();
    query->exec("select Fname, Lname, b.Card_no, bl.book_id, bl.branch_id from borrower b, book_loans bl where b.Card_no=bl.Card_no");
    QSqlQueryModel *model =  new QSqlQueryModel();
    model->setQuery(*query);
    ui->tableView_2->setModel(model);

    for(int i = 0; i < ui->tableView_2->model()->rowCount(); ++i)
    {
        bool match = false;
        for(int j = 0; j < ui->tableView_2->model()->columnCount(); ++j)
        {
            QModelIndex index = ui->tableView_2->model()->index(i, j, QModelIndex());
            QTableWidgetItem *item = new QTableWidgetItem(ui->tableView_2->model()->data(index).toString());

            if(item->text().contains(filter))
            {
                match = true;
                break;
            }
            delete item;

        }
        ui->tableView_2->setRowHidden(i, !match);
    }

}

/* check in  enter date */
void HomeWindow::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    DateIn *win = new DateIn();
    win->setParent(this);
    win->setBook_id(ui->tableView_2->model()->data(ui->tableView_2->model()->index(index.row(),3)).toString());
    win->setBranch_id(ui->tableView_2->model()->data(ui->tableView_2->model()->index(index.row(),4)).toString());
    win->setVisible(true);
}
QDate HomeWindow::getDateIn() const
{
    return dateIn;
}

void HomeWindow::setDateIn(const QDate &value)
{
    dateIn = value;
}

/*
 * refresh all tabs, reload data
 */
void HomeWindow::on_actionRefresh_triggered()
{
    // refresh table 3
    QSqlQuery *q3 = new QSqlQuery();
    q3->exec("select b.card_no, SUM(fine_amt), paid, Fname, Lname, address, phone from \
            fines f, book_loans bl, borrower b where f.loan_id=bl.loan_id and bl.card_no=b.card_no group by b.card_no");
    QSqlQueryModel *model3 = new QSqlQueryModel();

    model3->setQuery(*q3);
    ui->tableView_3->setModel(model3);

    // refresh table 2
    QSqlQuery *q2 = new QSqlQuery();
    q2->exec("select Fname, Lname, b.Card_no, bl.book_id, bl.branch_id, bl.date_int, bl.due_date from borrower b, book_loans bl where b.Card_no=bl.Card_no");
    QSqlQueryModel *model2 =  new QSqlQueryModel();
    model2->setQuery(*q2);
    ui->tableView_2->setModel(model2);

    // extra part: if the date in is null, set the whole row background red
    int rowCount = 0;
    QColor cellColor = Qt::red;
    while(q2->next()){
        if(q2->value(5).toString() ==  "NULL"){
            ui->tableView_2->model()->setData(ui->tableView_2->model()->index(rowCount, 2, QModelIndex()),cellColor, Qt::BackgroundRole);
        }
        rowCount ++;
    }

    // refresh table 1
    QSqlQuery *q1 = new QSqlQuery();
    q1->exec("select b.book_id,b.title,ba.author_name,ba.type,bc.branch_id,bc.no_of_copies+bav.no_of_avail_copies as total, bc.no_of_copies as available \
                from (book b join book_authors ba join book_copies bc)  join book_available bav \
                WHERE  b.book_id=ba.book_id and b.book_id=bc.book_id and b.book_id=bav.book_id and bav.branch_id=bc.branch_id;");

    QSqlQueryModel *model1 =  new QSqlQueryModel();
    model1->setQuery(*q1);
    ui->tableView->setModel(model1);
}
/*
 * start a fines management panel
 */
void HomeWindow::on_actionFines_Management_triggered()
{
    // load initial data

    QSqlQuery *q = new QSqlQuery();
    q->exec("select b.card_no, SUM(fine_amt), paid, Fname, Lname, address, phone from \
            fines f, book_loans bl, borrower b where f.loan_id=bl.loan_id and bl.card_no=b.card_no group by b.card_no");
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(*q);
    ui->tableView_3->setModel(model);

    ui->tabWidget->setCurrentIndex(2);

}
/*
 * initial fines (only need once)
 */
void HomeWindow::on_actionInitial_Fines_triggered()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query, q;
    query.exec("SELECT COUNT(*) FROM book_loans");      // get total number of book_loans
    int total;
    while(query.next()){
       total = query.value(0).toInt();
    }
    for(int i = 0; i < total; ++i){
        q.exec("INSERT INTO fines \
                   VALUES (" + QString::number(i + 1) + ", '0.00',0)");
    }
    QSqlDatabase::database().commit();
}
/*
 * a day has passed, recalculate all fines
 * calculate fines by today
 */

void HomeWindow::on_pushButton_7_clicked()
{
    // for every book_loan records, re- calculate a fine tuple for it.
    QSqlQuery query, q, q1;
    query.exec("SELECT COUNT(*) FROM book_loans");      // get total number of book_loans
    q1.exec("select * from book_loans");
    int total;
    while(query.next()){
       total = query.value(0).toInt();
    }

    int i = 0;
    while(q1.next()){
        qDebug() << i ++;
        q.exec("update fines set fine_amt='" +
               QString::number(FineCal::calculate(q1.value(5).toString(), q1.value(6).toString())) +
               "' where loan_id=" + QString::number(i)
               );
    }
}


/*
 * go to a particular date  **time traveling**
 */
void HomeWindow::on_pushButton_8_clicked()
{
    QString date_in = ui->dateEdit->date().toString("yyyy-MM-dd");
    QSqlQuery query, q, q1;
    query.exec("SELECT COUNT(*) FROM book_loans");      // get total number of book_loans
    q1.exec("select * from book_loans");
    int total;
    while(query.next()){
       total = query.value(0).toInt();
    }

    int i = 0;
    while(q1.next()){
        qDebug() << i ++;
        q.exec("update fines set fine_amt='" +
               QString::number(FineCal::timeTravel(q1.value(5).toString(), date_in)) +
               "' where loan_id=" + QString::number(i)
               );
    }
}

/*
 * Double click and allow user to edit data
 */
void HomeWindow::on_tableView_3_doubleClicked(const QModelIndex &index)
{
    EditCell *win = new EditCell();
    win->setColumn(index.column());
    win->setLoan_id(ui->tableView_3->model()->data(ui->tableView_3->model()->index(index.row(),0)).toInt());
    win->setData(ui->tableView_3->model()->data(ui->tableView_3->model()->index(index.row(),index.column())).toString());
    win->init();
    win->setVisible(true);
}

// show dockwidgets
void HomeWindow::on_actionSearch_triggered()
{
    ui->dockWidget_2->show();
    ui->dockWidget_3->show();
}

void HomeWindow::on_actionBorrower_Management_triggered()
{
    ui->dockWidget_4->show();
}

void HomeWindow::on_actionCheck_Out_triggered()
{
    ui->dockWidget_5->show();
}

void HomeWindow::on_actionCheck_In_triggered()
{
    ui->dockWidget_6->show();
    ui->tabWidget->setCurrentIndex(1);
}

/* perform global search on table */
void HomeWindow::on_pushButton_6_clicked()
{
    QString filter = ui->lineEdit_14->text();

    for(int i = 0; i < ui->tableView_3->model()->rowCount(); ++i)
    {
        bool match = false;
        for(int j = 0; j < ui->tableView_3->model()->columnCount(); ++j)
        {
            QModelIndex index = ui->tableView_3->model()->index(i, j, QModelIndex());
            QTableWidgetItem *item = new QTableWidgetItem(ui->tableView_3->model()->data(index).toString());

            if(item->text().contains(filter))
            {
                match = true;
                break;
            }
            delete item;

        }
        ui->tableView_3->setRowHidden(i, !match);
    }
}
