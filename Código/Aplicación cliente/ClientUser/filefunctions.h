#ifndef FILEFUNCTIONS
#define FILEFUNCTIONS

#include "dateClass.h"


class FileData{
    string user;
    string host;
    int port;
    string dir;
    dateClass lastDate;
    string fileExtension;
    int seconds;
    string lastRefresh;

public:
    /*FileData(string dir, string fileExtension, string host, dateClass lastDate, int port, int seconds, string user, string) :
    dir(dir), fileExtension(fileExtension), host(host), lastDate(lastDate), port(port), seconds(seconds), user(user), lastRefresh() {
    }*/

    FileData() :
    dir(""), fileExtension(""), host(""), lastDate(), port(0), seconds(0), user(""), lastRefresh("") {
    }


    string GetDir() const {
        return dir;
    }

    void SetDir(string dir) {
        this->dir = dir;
    }

    string GetFileExtension() const {
        return fileExtension;
    }

    void SetFileExtension(string fileExtension) {
        this->fileExtension = fileExtension;
    }

    string GetHost() const {
        if(host == "")
            return "null";
        return host;
    }

    void SetHost(string host) {
        this->host = host;
    }

    dateClass GetLastDate() const {
        return lastDate;
    }

    void SetLastDate(dateClass lastDate) {
        this->lastDate = lastDate;
    }

    int GetPort() const {
        return port;
    }

    string GetPortS() const {
        stringstream ss;
        ss << port;
        return ss.str();
    }

    void SetPort(int port) {
        this->port = port;
    }

    int GetSeconds() const {
        return seconds;
    }

    string GetSecondsSS() const {
        stringstream ss;
        ss << seconds;

        return ss.str();
    }

    string GetSecondsS() const {
        stringstream s1,s2,s3;
        int hour = (seconds / 60 / 60) % 24;
        int minute = (seconds / 60) % 60;
        int second = seconds % 60;
        s1 << hour;
        s2 << minute;
        s3 << second;


        string ret = "";
        if(hour > 0)
            if(hour == 1)
                ret.append(s1.str() + " hora ");
            else
                ret.append(s1.str() + " horas ");

        if(minute > 0)
            if(minute == 1)
                ret.append(s2.str() + " minuto ");
            else
                ret.append(s2.str() + " minutos ");

        if(second > 0)
            if(second == 1)
                ret.append(s3.str() + " segundo");
            else
                ret.append(s3.str() + " segundos");

        return ret;
    }

    void SetSeconds(int seconds) {
        this->seconds = seconds;
    }

    string GetUser() const {
        if(user == "")
            return "null";

        return user;
    }

    void SetUser(string user) {
        this->user = user;
    }

    void SetLastRefresh(string refresh){
        this->lastRefresh = refresh;
    }

    string GetLastRefresh(){
        return lastRefresh;
    }

};

bool isValidDate(string dateInput){
    //regex date("^(19|20)\d\d[-]([1-9]|1[012])[-]([1-9]|[12][0-9]|3[01])$");
    //regex date("^(19|20)[1-9][1-9][-]([1-9]|1[012])[-]([1-9]|[12][0-9]|3[01])$");
    regex date("^(0[1-9]|[12][0-9]|3[01])[_](ene|feb|mar|abr|may|jun|jul|ago|sep|oct|nov|dic)[_](19|20)[1-9][1-9]$");
      if (regex_match(dateInput, date)) {
        // At this point, $1 holds the year, $2 the month and $3 the day of the date entered
        dateClass d = dateClass(dateInput);
        if (d.GetDay() == 31 && (d.GetMonth() == 4 || d.GetMonth() == 6 || d.GetMonth() == 9 || d.GetMonth() == 11)) {
          return false; // 31st of a month with 30 days
        } else if (d.GetDay() >= 30 && d.GetMonth() == 2) {
          return false; // February 30th or 31st
        } else if (d.GetMonth() == 2 && d.GetDay() == 29 && !(d.GetYear() % 4 == 0 && (d.GetYear() % 100 != 0 || d.GetYear() % 400 == 0))) {
          return false; // February 29th outside a leap year
        } else {
            qDebug() << string("Valida " + dateInput).c_str();
          return true; // Valid date
        }
      } else {
          qDebug() << string("La expresion ha petado " + dateInput).c_str();
        return false; //Not a date
      }
}


void modifyConfig(FileData dataInfo) {

    string rootDirTemp = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toUtf8().constData();
            rootDirTemp.append("/EmergiendoConElSol/Setup.conf.temp");

    string rootDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toUtf8().constData();
            rootDir.append("/EmergiendoConElSol/Setup.conf");


    ofstream fs(rootDirTemp, std::ofstream::out);
    fs << "#######################################################" << endl;
    fs << "				Creado por" << endl;
    fs << "		Jesús Alberto Salazar Ortega" << endl;
    fs << "			salazarcontactinfo@gmail.com" << endl;
    fs << "		=======================" << endl;
    fs << "##Archivo de configuración para los distintos usuarios" << endl;
    fs << "Nombre del archivo:Setup.conf" << endl;
    fs << "Directory:" + dataInfo.GetDir() << endl;
    fs << "User:" + dataInfo.GetUser() << endl;
    fs << "Host:" + dataInfo.GetHost() << endl;
    fs << "Port:" + dataInfo.GetPortS() << endl;
    fs << "Refresh interval:" + dataInfo.GetSecondsSS() << endl;
    fs << "File extension:" + dataInfo.GetFileExtension() << endl;
    fs << "Last refresh:" + dataInfo.GetLastDate().toString() << endl;
    fs.close();




    /*ifstream fs(filename.c_str());
    ofstream fo((filename + ".temp").c_str(), std::ofstream::out);
    char cadena[128];
  //  UserInfo actual;
    int size;

    for (int i = 0; i < 10; i++) {
        fs.getline(cadena, 128);
        fo << cadena << endl;
    }

    //for (int i = 0; i < user.size(); i++) {
  //      fo << "Usuario" << newUser(i + 1) << ":" << user.at(i).GetName() << endl;
   //     fo << "Central:" << user.at(i).GetCentral() << endl;
    //}*/

    qDebug() << rootDir.c_str();
    remove(rootDir.c_str());
    rename(rootDirTemp.c_str(), rootDir.c_str());
}

FileData loadConfig() {

    string rootDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toUtf8().constData();
            rootDir.append("/EmergiendoConElSol/Setup.conf");

    ifstream fs(rootDir);
    char cadena[128];
    FileData filedata = FileData();

    for (int i = 0; i < 7; i++) {
        fs.getline(cadena, 128);
    }

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);
    filedata.SetDir(string(cadena));

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);
    filedata.SetUser(string(cadena));

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);
    filedata.SetHost(string(cadena));

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);

    filedata.SetPort(atoi(cadena));

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);
    filedata.SetSeconds(atoi(cadena));

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);
    filedata.SetFileExtension(string(cadena));

    fs.getline(cadena, 128, ':');
    fs.getline(cadena, 128);
    filedata.SetLastDate(dateClass(cadena));




    fs.close();
    return filedata;
}


void createConfigFile() {

    string rootDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toUtf8().constData();
            rootDir.append("/EmergiendoConElSol/Setup.conf");
    ofstream fs(rootDir, std::ofstream::out);
    fs << "#######################################################" << endl;
    fs << "				Creado por" << endl;
    fs << "		Jesús Alberto Salazar Ortega" << endl;
    fs << "			salazarcontactinfo@gmail.com" << endl;
    fs << "		=======================" << endl;
    fs << "##Archivo de configuración para los distintos usuarios" << endl;
    fs << "Nombre del archivo:Setup.conf" << endl;
    fs << "Directory:" << endl;
    fs << "User:" << endl;
    fs << "Host:" << endl;
    fs << "Port:" << endl;
    fs << "Refresh interval:" << endl;
    fs << "File extension:" << endl;
    fs << "Last refresh:" << endl;
    fs.close();
}

bool OpenCppFileExists(const string& filename) {
    fstream fin;

    //this will fail if more capabilities to read the
    //contents of the file is required (e.g. \private\...)
    fin.open(filename.c_str(), ios::in);

    if (fin.is_open()) {
        fin.close();
        return true;
    }
    fin.close();

    return false;
}
#endif // FILEFUNCTIONS


