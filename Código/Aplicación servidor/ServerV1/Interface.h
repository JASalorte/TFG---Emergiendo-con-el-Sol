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
#include "FileFunctions.h"


using namespace std;

void loadedUsersUI(const vector<UserInfo>& user) {
    cout << "Se han cargado " << user.size() << " usuarios del archivo de texto" << endl;
    for (int i = 0; i < user.size(); i++) {
        if (user.at(i).GetCentral() != "")
            cout << "Central: " << user.at(i).GetCentral() << endl;
        else
            cout << "Central: Sin nombre definido" << endl;
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
}

void userFirstComprobationUI(const vector<UserInfo>& user,string host,string port) {
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
            command = "sudo useradd " + user.at(i).GetName();
            linux_return_function((const char*) command.c_str());
            linux_return_function(string("sudo chmod 770 -R /home/" + user.at(i).GetName()).c_str());
            linux_return_function(string("sudo chgrp -R shared /home/" + user.at(i).GetName()).c_str());


            command = "mkdir -p -m 770 /home/" + user.at(i).GetName() + "/.ssh";
            linux_return_function((const char*) command.c_str());

            command = "mkdir -p -m 770 /home/" + user.at(i).GetName() + "/workdir";
            linux_return_function((const char*) command.c_str());

            command = "ssh-keygen -t rsa -N Pablito -C salazarcontactinfo@gmail.com -f /home/" + user.at(i).GetName() + "/.ssh/id_rsa";
            linux_return_function((const char*) command.c_str());
            command = "cat /home/" + user.at(i).GetName() + "/.ssh/id_rsa.pub | tee -a /home/" + user.at(i).GetName() + "/.ssh/authorized_keys";
            linux_return_function((const char*) command.c_str());

            linux_return_function(string("sudo chmod 750 -R /home/" + user.at(i).GetName()).c_str());
            linux_return_function(string("sudo chgrp -R shared /home/" + user.at(i).GetName()).c_str());
            command = "sudo chown -R " + user.at(i).GetName() + " /home/" + user.at(i).GetName();
            linux_return_function((const char*) command.c_str());

            num++;
        }


        command = "[ -f /var/www/Installer/\"" + user.at(i).GetCentral() + ".zip\" ] && echo \"Found\" || echo \"Not found\"";
        vector<string> checkInstaller = linux_return_function((const char*) command.c_str());

        if (checkInstaller[0].compare("Found\n") != 0) {
            cout << "Creando instalador..." << endl;
            command = "cp -r $HOME/Installer/Base/* $HOME/Installer/Temp";
            linux_return_function((const char*) command.c_str());

            command = "cp /home/" + user.at(i).GetName() + "/.ssh/id_rsa $HOME/Installer/Temp/keys";
            linux_return_function((const char*) command.c_str());
            command = "cp /home/" + user.at(i).GetName() + "/.ssh/id_rsa.pub $HOME/Installer/Temp/keys";
            linux_return_function((const char*) command.c_str());


            ofstream fs;
            fs.open("../Installer/Temp/Setup.conf", std::ios::out);
            fs << "#######################################################" << "\r\n";
            fs << "				Creado por" << "\r\n";
            fs << "		Jesús Alberto Salazar Ortega" << "\r\n";
            fs << "			salazarcontactinfo@gmail.com" << "\r\n";
            fs << "		=======================" << "\r\n";
            fs << "##Archivo de configuración para los distintos usuarios" << "\r\n";
            fs << "Nombre del archivo:Setup.conf" << "\r\n";
            fs << "Directory:workdir" << "\r\n";
            fs << "User:" << user.at(i).GetName() << "\r\n";
            fs << "Host:" << host << "\r\n";
            fs << "Port:" << port << "\r\n";
            fs << "Refresh interval:15" << "\r\n";
            fs << "File extension:csv" << "\r\n";
            fs << "Last refresh:01_ene_1901" << "\r\n";

            fs.close();
            
            command = "sudo chmod 775 ../Installer/Temp/Setup.conf";
            linux_return_function((const char*) command.c_str());
            


            command = "makensis $HOME/Installer/NSISscript.nsi";
            linux_return_function((const char*) command.c_str());

            command = "rm -r $HOME/Installer/Temp/*";
            linux_return_function((const char*) command.c_str());

            command = "cp $HOME/Installer/installer.exe /var/www/Installer/\"" + user.at(i).GetCentral() + ".exe\"";
            linux_return_function((const char*) command.c_str());

            command = "zip -j /var/www/Installer/\"" + user.at(i).GetCentral() + ".zip\" /var/www/Installer/\"" + user.at(i).GetCentral() + ".exe\" /var/www/Installer/Instrucciones\\ Instalación.txt";
            linux_return_function((const char*) command.c_str());
            
            command = "rm /var/www/Installer/\"" + user.at(i).GetCentral() + ".exe\"";
            linux_return_function((const char*) command.c_str());
        }




    }
    if (num == 0) {
        cout << "Estan todos los usuarios" << endl;
    }
}

#endif	/* INTERFACE_H */

