/* 
 * File:   Interface.h
 * Author: Linkku
 *
 * Created on 22 de abril de 2015, 14:06
 */

#ifndef INTERFACE_H
#define	INTERFACE_H

#include "algorithm"
#include <dirent.h>

#include "UserInfo.h"
#include "dateClass.h"
#include "dirClass.h"
#include "FileFunctions.h"


using namespace std;

void loadedUsersUI(const vector<UserInfo>& user) {
    cout << "Se han cargado " << user.size() << " usuarios del archivo de texto" << endl;
    for (int i = 0; i < user.size(); i++) {
        if (user.at(i).GetCentral() != "")
            cout << "Central: " << user.at(i).GetCentral() << endl;
        else
            cout << "Central: " << " Sin nombre definido" << endl;
    }

    cout << endl << endl;
}

vector<UserInfo> checkCentralNames(const vector<UserInfo>& users) {
    vector<UserInfo> user(users);
    string var;
    for (int i = 0; i < user.size(); i++) {
        for (int j = i + 1; j < user.size(); j++) {
            while (user.at(i).GetCentral() == user.at(j).GetCentral()) {
                cout << "El nombre de la central del usuario " << user.at(i).GetName() << " es idéntico al del usuario " << user.at(j).GetName() << "." << endl;
                cout << "Introduzca un nuevo nombre de central para el usuario " << user.at(j).GetName() << ": ";
                cin >> var;
                std::cin.clear();
                std::cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
                user.at(j).SetCentral(var);
                cout << endl;
            }
        }
    }
    return user;
}

vector<UserInfo> centralComprobationUI(const vector<UserInfo>& users) {
    vector<UserInfo> user(users);
    for (int i = 0; i < user.size(); i++) {
        if (user.at(i).GetCentral() == "") {
            string var;
            cout << "Introduzca un nombre para la central del usuario " << user.at(i).GetName() << ": " << endl;
            cin >> var;
            std::cin.clear();
            std::cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            user.at(i).SetCentral(var);
            cout << endl;
        }
    }

    return checkCentralNames(user);
    //modifyUsers(user, filename);
}

void userFirstComprobationUI(const vector<UserInfo>& user) {
    cout << "Vamos a comprobar que todos los usuarios existen ya" << endl;
    std::vector<std::string> cadena;

    if (user.size() == 0) {
        cout << "No hay ningún usuario." << endl;
    }

    int num = 0;
    for (int i = 0; i < user.size(); i++) {
        string command = "cat /etc/passwd | grep " + user.at(i).GetName();
        cadena = linux_return_function((const char*) command.c_str());
        if (cadena.size() == 0) {
            cout << "El usuario de la central: " << user.at(i).GetCentral() << " no existe, se procede a crearlo." << endl;
            linux_return_function(string("sudo useradd " + user.at(i).GetName()).c_str());
            linux_return_function(string("sudo chmod 777 /home/" + user.at(i).GetName()).c_str());
            command = "sudo mkdir -p /home/" + user.at(i).GetName() + "/.ssh";
            linux_return_function((const char*) command.c_str());
            command = "sudo chmod 700 /home/" + user.at(i).GetName() + "/.ssh";
            linux_return_function((const char*) command.c_str());
            num++;
        }
    }
    if (num == 0) {
        cout << "Estan todos los usuarios" << endl;
    }
}

void configureKeyPair(const string& username) {
    std::vector<std::string> cadena;
    string command;

    //Creamos el directorio donde se van a almacenar las claves
    command = "sudo mkdir -p /home/" + username + "/.ssh";
    linux_return_function((const char*) command.c_str());
    command = "sudo mkdir -p /home/" + username + "/workdir";
    linux_return_function((const char*) command.c_str());
    command = "sudo chmod -R 777 /home/" + username + "/workdir";
    linux_return_function((const char*) command.c_str());
    command = "sudo chown " + username + " /home/" + username + "/.ssh";
    linux_return_function((const char*) command.c_str());
    command = "sudo chmod 700 /home/" + username + "/.ssh";
    linux_return_function((const char*) command.c_str());
    command = "sudo ssh-keygen -t rsa -N Pablito -C salazarcontactinfo@gmail.com -f /home/" + username + "/.ssh/id_rsa";
    linux_return_function((const char*) command.c_str());
    command = "sudo cat /home/" + username + "/.ssh/id_rsa.pub | sudo tee -a /home/" + username + "/.ssh/authorized_keys";
    linux_return_function((const char*) command.c_str());
    command = "sudo chown " + username + " /home/" + username + "/.ssh/authorized_keys";
    linux_return_function((const char*) command.c_str());
    command = "sudo chmod 600 /home/" + username + "/.ssh/authorized_keys";
    linux_return_function((const char*) command.c_str());
    command = "sudo mkdir -p $HOME/keys/" + username;
    linux_return_function((const char*) command.c_str());
    command = "sudo cp /home/" + username + "/.ssh/id_rsa $HOME/keys/" + username;
    linux_return_function((const char*) command.c_str());
    command = "sudo cp /home/" + username + "/.ssh/id_rsa.pub $HOME/keys/" + username;
    linux_return_function((const char*) command.c_str());
}

void userCheckKeysUI(const vector<UserInfo>& user) {
    cout << "Vamos a comprobar que los key pairs esten." << endl;
    std::vector<std::string> cadena;

    if (user.size() == 0) {
        cout << "No hay ningún usuario." << endl;
    }
    bool exist;
    int n = 0;

    for (int i = 0; i < user.size(); i++) {

        exist = false;

        string command = "sudo ls /home/" + user.at(i).GetName() + "/.ssh";
        cadena = linux_return_function((const char*) command.c_str());
        for (int j = 0; j < cadena.size(); j++) {
            if (cadena.at(j).compare("authorized_keys")) {
                exist = true;
            }
        }

        if (!exist) {
            n++;
            cout << "El usuario " << user.at(i).GetName() << " no esta configurado, se hara automaticamente." << endl;
            configureKeyPair(user.at(i).GetName());
        }
    }
    if (n == 0) {
        cout << "Las keys estan bien configuradas." << endl;
    }
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
                rdir.SetDate(dateClass(string(pdir->d_name).substr(0, pos)));
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

bool dateComparator(dirClass i, dirClass j) {
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

vector<dirClass> filesSinceDate(string directory, string filetype, string date) {
    //dateClass datefind = dateClass(date);
    vector<dirClass> v, vSorted;
    list_recur(directory, filetype, v);

    for (int i = 0; i < v.size(); i++) {
        if (dateComparator(v.at(i), dirClass(date, "", ""))) {
            vSorted.push_back(v.at(i));
        }
    }
    sort(vSorted.begin(), vSorted.end(), dateComparator);

    /*for (int i = 0; i < vSorted.size(); i++) {
        cout << vSorted.at(i).GetFullDir() << endl;
    }*/

    return vSorted;
}

bool fileToDDBB(string fulldir) {

    cout << "DDBB: " + fulldir << endl;
    return true;
}

void upToDate(vector<UserInfo> user) {
    //vector<dirClass> dates;

    dirClass temp;
    cout << "Acabamos de entrar" << endl;
    for (int i = 0; i < user.size(); i++) {
        temp.SetDir("/home/" + user.at(i).GetName() + "/workdir");
        temp.SetFile("User.conf");
        cout << "Estamos despues de temp" << endl;
        if (OpenCppFileExists(temp.GetFullDir())) {
            cout << "Existia el archivo" << endl;
            ifstream fs(temp.GetFullDir().c_str());
            char cadena[128];

            for (int j = 0; j < 7; j++) {
                fs.getline(cadena, 128);
            }

            fs.getline(cadena, 128, ':');
            fs.getline(cadena, 128);
            user.at(i).SetDate(string(cadena));
            temp.SetDate(dateClass(string(cadena)));

            fs.getline(cadena, 128, ':');
            fs.getline(cadena, 128);

            user.at(i).SetRefresh(atof(cadena));

            fs.close();
            cout << "Leimos el archivo creado" << endl;
        } else {
            cout << "No existia el archivo" << endl;
            cout << "Se supone que se ha creado un archivo en: " + temp.GetFullDir() << endl;
            ofstream fs(temp.GetFullDir().c_str(), std::ofstream::out);
            if (fs.fail()) {
                cout << "Could not write the file" << flush;
            }
            fs << "#######################################################" << endl;
            fs << "				Creado por" << endl;
            fs << "		Jesús Alberto Salazar Ortega" << endl;
            fs << "			salazarcontactinfo@gmail.com" << endl;
            fs << "		=======================" << endl;
            fs << "##Archivo de configuración para el usuario" << endl;
            fs << "Nombre del archivo:User.conf" << endl;
            fs << "Ultima actualizacion:1-1-1" << endl;
            fs << "Tiempo de refresco en minutos:5.0" << endl;
            temp.SetDate(dateClass("1-1-1"));
            user.at(i).SetDate("1-1-1");
            user.at(i).SetRefresh(5.0);
            fs.close();
            cout << "Terminamos de crear el archivo" << endl;
        }

        cout << "Vamos a coger el sistema de archivos" << endl;
        vector<dirClass> v = filesSinceDate(temp.GetDir(), "csv", temp.GetStringDate());
        cout << "Sistema de archivos cogido" << endl;

        if (v.size() == 0) {
            cout << "Mierda" << endl;
        }

        string lastDate = temp.GetStringDate();
        int j = 0;
        bool error = false;
        while (0 < v.size() && !error) {
            if (fileToDDBB(v.at(v.size() - 1).GetFullDir())) {
                lastDate = v.at(v.size() - 1).GetStringDate();
                v.pop_back();
                //j++;
            } else {
                error = true;
            }
        }

        ifstream fs(temp.GetFullDir().c_str());
        ofstream fo((temp.GetFullDir() + ".temp").c_str(), std::ofstream::out);
        char cadena[128];


        for (int j = 0; j < 7; j++) {
            fs.getline(cadena, 128);
            fo << cadena << endl;
        }

        fo << "Ultima actualizacion:" << lastDate << endl;
        fo << "Tiempo de refresco en minutos:" << user.at(i).GetRefresh() << endl;

        rename((temp.GetFullDir() + ".temp").c_str(), temp.GetFullDir().c_str());

    }






}



#endif	/* INTERFACE_H */

