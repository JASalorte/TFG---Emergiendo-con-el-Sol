/* 
 * File:   UserInfo.h
 * Author: Linkku
 *
 * Created on 22 de abril de 2015, 13:16
 */

#ifndef USERINFO_H
#define	USERINFO_H

using namespace std;

class UserInfo {
private:
    int number;
    string name;
    string central;
    string date;
    float refresh;
public:

    UserInfo() {
        number = -1;
        name = "";
        central = "";
        date = "1-1-1";
        refresh = 5;

    }

    UserInfo(int n, string na, string c) {
        number = n;
        name = na;
        central = c;
        date = "1-1-1";
        refresh = 5;
    }
    
    UserInfo(int n, string na, string c, string d, float r) {
        number = n;
        name = na;
        central = c;
        date = d;
        refresh = r;
    }

    void Set(int n, string na, string c) {
        number = n;
        name = na;
        central = c;
    }
    
    void Set(int n, string na, string c, string d, float r) {
        number = n;
        name = na;
        central = c;
        date = d;
        refresh = r;
    }

    UserInfo(const UserInfo& orig) {
        this->number = orig.number;
        this->name = orig.name;
        this->central = orig.central;
        this->date = orig.date;
        this->refresh = orig.refresh;

    }

    string GetCentral() const {
        return central;
    }

    void SetCentral(string central) {
        this->central = central;
    }

    string GetName() const {
        return name;
    }

    void SetName(string name) {
        this->name = name;
    }

    int GetNumber() const {
        return number;
    }

    void SetNumber(int number) {
        this->number = number;
    }
    
    string GetDate() const {
        return date;
    }

    void SetDate(string date) {
        this->date = date;
    }

    float GetRefresh() const {
        return refresh;
    }

    void SetRefresh(float refresh) {
        this->refresh = refresh;
    }

    ~UserInfo() {
    }


};

#endif	/* USERINFO_H */

