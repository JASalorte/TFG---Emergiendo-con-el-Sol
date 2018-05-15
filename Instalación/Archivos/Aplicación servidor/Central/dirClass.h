/* 
 * File:   dateClass.h
 * Author: Linkku
 *
 * Created on 11 de mayo de 2015, 15:27
 */

#ifndef DIRCLASS_H
#define	DIRCLASS_H

#include <string>
#include "dateClass.h"


using namespace std;

class dirClass {
private:
    dateClass date;
    string dir;
    string file;
public:

    dirClass() {
        date.SetDay(0);
        date.SetMonth(0);
        date.SetYear(0);
        dir = file = "";
    }

    dirClass(string date, string dir, string file) {
        string temp;
        size_t f = date.find("-");
        temp = date.substr(0, f);
        this->date.SetYear(atoi(temp.c_str()));
        date.replace(0, f + 1, "");

        f = date.find("-");
        temp = date.substr(0, f);
        this->date.SetMonth(atoi(temp.c_str()));
        date.replace(0, f + 1, "");

        this->date.SetDay(atoi(date.c_str()));

        this->dir = dir;
        this->file = file;
    }

    string GetFullDir() {
        return dir + "/" + file;
    }

    dateClass GetDate() const {
        return date;
    }

    string GetStringDate() const {
        stringstream ss;
        
        ss << date.GetYear();
        string str = ss.str();
        str.append("-");
        ss.str(std::string());
        
        ss << date.GetMonth();
        str.append(ss.str());
        str.append("-");
        ss.str(std::string());
        
        ss << date.GetDay();
        str.append(ss.str());

        return str;
    }

    void SetDate(dateClass date) {
        this->date = date;
    }

    string GetDir() const {
        return dir;
    }

    void SetDir(string dir) {
        this->dir = dir;
    }

    string GetFile() const {
        return file;
    }

    void SetFile(string file) {
        this->file = file;
    }

};


#endif	/* DIRCLASS_H */

