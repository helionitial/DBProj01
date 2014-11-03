#include "finecal.h"
#include <QDate>
FineCal::FineCal()
{
}

/*
 * Format of the date should be:
 * yyyy-MM-dd
 */
double FineCal::calculate(QString due, QString date_in)
{
    QDate d1 = QDate::fromString(due, "yyyy-MM-dd");
    if(date_in.isNull() || date_in.isEmpty() || date_in == "NULL"){
        QDate today = QDate::currentDate();
        if(today.daysTo(d1) < 0){   // need pay fine
            return d1.daysTo(today) * 0.25;
        }else{
            return 0;
        }
    }

    QDate d2 = QDate::fromString(date_in, "yyyy-MM-dd");

    if(d2.daysTo(d1) < 0){  // need pay fine
        return (d1.daysTo(d2) * 0.25);
    }else{  // no fine
        return 0;
    }
}

double FineCal::timeTravel(QString due, QString date_in)
{
    QDate d1 = QDate::fromString(due, "yyyy-MM-dd");
    QDate d2 = QDate::fromString(date_in, "yyyy-MM-dd");

    if(d2.daysTo(d1) < 0){  // need pay fine, travel to future
        return (d1.daysTo(d2) * 0.25);
    }else{  // no fine, travel to past
        return 0;
    }
}
