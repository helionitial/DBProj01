#include <QString>

#ifndef FINECAL_H
#define FINECAL_H

class FineCal
{
public:
    FineCal();
    static double calculate(QString due, QString date_in);
    static double timeTravel(QString due, QString date_in);
};

#endif // FINECAL_H
