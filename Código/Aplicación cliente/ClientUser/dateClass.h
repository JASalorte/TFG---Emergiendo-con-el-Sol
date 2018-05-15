/* 
 * File:   dateClass.h
 * Author: Linkku
 *
 * Created on 11 de mayo de 2015, 15:27
 */

#ifndef DATECLASS_H
#define	DATECLASS_H

#include <string>

using namespace std;



class dateClass {
private:
    int year, month, day;

    int monthToInt(string month){
        if(month == "ene")
            return 1;
        if(month == "feb")
            return 2;
        if(month == "mar")
            return 3;
        if(month == "abr")
            return 4;
        if(month == "may")
            return 5;
        if(month == "jun")
            return 6;
        if(month == "jul")
            return 7;
        if(month == "ago")
            return 8;
        if(month == "sep")
            return 9;
        if(month == "oct")
            return 10;
        if(month == "nov")
            return 11;
        if(month == "dic")
            return 12;
    }

    string intToMonth(int month){
        if(month == 1)
            return "ene";
        if(month == 2)
            return "feb";
        if(month == 3)
            return "mar";
        if(month == 4)
            return "abr";
        if(month == 5)
            return "may";
        if(month == 6)
            return "jun";
        if(month == 7)
            return "jul";
        if(month == 8)
            return "ago";
        if(month == 9)
            return "sep";
        if(month == 10)
            return "oct";
        if(month == 11)
            return "nov";
        if(month == 12)
            return "dic";
    }

public:

    dateClass() {
        year = month = day = 0;
    }

    dateClass(string date) {
        /*string temp;

        size_t f = date.find("_");
        temp = date.substr(0, f);
        year = atoi(temp.c_str());
        date.replace(0, f + 1, "");

        f = date.find("_");
        temp = date.substr(0, f);
        month = atoi(temp.c_str());
        date.replace(0, f + 1, "");

        day = atoi(date.c_str());*/


        string temp;
        size_t f = date.find("_");
        temp = date.substr(0, f);
        day = atoi(temp.c_str());
        date.replace(0, f + 1, "");

        f = date.find("_");
        temp = date.substr(0, f);
        month = monthToInt(temp.c_str());
        date.replace(0, f + 1, "");

        year = atoi(date.c_str());
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

    string toString() {
        stringstream s1,s3;
        s1 << year;
        s3 << day;


        string d = s3.str();
        if(d.length() == 1)
            d = "0" + d;

        return string(d + "_" + intToMonth(month) + "_" + s1.str());

    }



};


#endif	/* DATECLASS_H */

