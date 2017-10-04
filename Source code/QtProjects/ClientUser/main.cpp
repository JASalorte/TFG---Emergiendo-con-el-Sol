//#include <winsock2.h>
#define strtoll     _strtoi64
#define strtoull    _strtoui64

#include <stdio.h>
#include <string.h>
#include <QApplication>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <dirent.h>
#include <limits>
#include <vector>
#include <stdlib.h>
#include <libssh/libssh.h>

#include "mainwindow.h"
#include "SSHFunctions.h"
#include "dateClass.h"
#include "dirClass.h"




/*bool dateComparator(dirClass i, dirClass j) {
    if (i.GetDate().GetYear() < j.GetDate().GetYear())
        return false;
    if (i.GetDate().GetYear() > j.GetDate().GetYear())
        return true;

    if (i.GetDate().GetMonth() < j.GetDate().GetMonth())
        return false;
    if (i.GetDate().GetMonth() > j.GetDate().GetMonth())
        return true;

    if (i.GetDate().GetDay() < j.GetDate().GetDay())
        return false;
    if (i.GetDate().GetDay() > j.GetDate().GetDay())
        return true;

    if (i.GetDate().GetDay() == j.GetDate().GetDay())
        return true;
}

vector <dirClass> list_recur(string directory, string filetype, vector<dirClass> &v) {
    DIR* dir;
    dirent* pdir;
    dirClass rdir;

    dir = opendir(directory.c_str());
    while (pdir = readdir(dir)) {
        if (string(pdir->d_name).compare(".") && string(pdir->d_name).compare("..")) {
            int pos = string(pdir->d_name).rfind("." + filetype);
            if (pos != std::string::npos) {
                rdir.SetDate(string(pdir->d_name).substr(0, pos));
                rdir.SetDir(directory);
                rdir.SetFile(string(pdir->d_name));
                v.push_back(rdir);
            }
            if (pdir->d_type == DT_DIR) {
                list_recur(directory + "\\" + pdir->d_name, filetype, v);
            }
        }
    }

    return v;
}

vector<dirClass> filesSinceDate(string directory, string filetype, string date) {
    //dateClass datefind = dateClass(date);
    vector<dirClass> v, vSorted;
    list_recur(directory, filetype, v);

    for (int i = 0; i < v.size(); i++) {
        if (dateComparator(v.at(i), dirClass(date, "", ""))) {
            vSorted.push_back(v.at(i));
        }
    }

    qSort(vSorted.begin(), vSorted.end(), dateComparator);
    return vSorted;
}*/

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    QString arq;
    /*arq="lock.ctl";
    QDir d("C:\\windows\\system32\\");
    d.remove(arq);
    return -1;*/

    #ifdef Q_OS_WIN32
        arq="C:\\windows\\system32\\lock.ctl";
    #else
        arq="/var/run/lock.ctl";
    #endif
    QFile dlock(arq);
    if(dlock.exists()) {
        //QMessageBox::critical(w,"MyApp", ".this programs already is running", QMessageBox::Close);
        QApplication::restoreOverrideCursor();
        return -1;
    }
    else {
        dlock.open(QFile::WriteOnly | QFile::Truncate);
        QTextStream out(&dlock);
        out << "running" << endl;
        dlock.close();
    }

    string host = "150.214.174.39";
    int port = 10022;
    string username = "user0002";
    string lastDay = "2015-5-6";
    //vector<dirClass> v = filesSinceDate("workdir", "csv", lastDay);



    w.show();

    return a.exec();
}
