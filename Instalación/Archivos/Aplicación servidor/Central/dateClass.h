/* 
 * File:   dateClass.h
 * Author: Linkku
 *
 * Created on 11 de mayo de 2015, 15:27
 */

#ifndef DATECLASS_H
#define	DATECLASS_H

using namespace std;



class dateClass {
private:
    int year, month, day;

public:

    dateClass() {
        year = month = day = 0;
    }

    dateClass(string date) {
        string temp;

        size_t f = date.find("-");
        temp = date.substr(0, f);
        year = atoi(temp.c_str());
        date.replace(0, f + 1, "");

        f = date.find("-");
        temp = date.substr(0, f);
        month = atoi(temp.c_str());
        date.replace(0, f + 1, "");

        day = atoi(date.c_str());

    }

    int GetDay() const {
        return day;
    }

    int GetMonth() const {
        return month;
    }

    int GetYear() const {
        return year;
    }

    void SetDay(int day) {
        this->day = day;
    }

    void SetMonth(int month) {
        this->month = month;
    }

    void SetYear(int year) {
        this->year = year;
    }



};


#endif	/* DATECLASS_H */

