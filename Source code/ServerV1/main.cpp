/* 
 * File:   main.cpp
 * Author: Linkku
 *
 * Created on 30 de enero de 2015, 11:49
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <unistd.h>

#include "FileFunctions.h"
#include "LinuxCalls.h"
#include "UserInfo.h"
#include "Interface.h"

using namespace std;

int main() {

    vector<UserInfo> user;

    /*Ejemplo de como se usa el linux return*/

    /*std::vector<std::string> cadena = linux_return_function((char*) "ls -l");
    for(int i = 0; i < cadena.size(); i++){
        std::cout << "Index: " << i << " " << cadena.at(i);
    }
    cadena.clear();*/

    cout << "#################### Iniciando el servidor V0.1 ####################" << endl << endl;

    string initial_dir = "./Main.conf";


    /*if (!OpenCppFileExists(initial_dir)) {
        cout << "El archivo Main.conf no existe\n" <<
                "Se está iniciando el servidor por primera vez\n" <<
                "¿Desea crearlo?\n1.- Si\n2.- No" << endl;
        if (getchar() == '1') {
            int var = 0;
            while (var <= 0) {
                cout << "¿Cuantos usuarios desea crear inicialmente?" << endl;
                cin >> var;
                std::cin.clear();
                std::cin.ignore(numeric_limits<std::streamsize>::max(), '\n');

                if (var <= 0)
                    cout << "Tiene que introducir un número mayor que 0" << endl;
                
                cout << endl << endl;
            }
            createConfigFile(var);
        } else {
            cout << "No se puede continuar sin una configuración.\nSaliendo..." << endl;
            return 0;
        }
    }*/

    cout << "#############Cargando archivo de configuración Main.conf.##################" << endl << endl;
    user = loadConfig(initial_dir);
    loadedUsersUI(user);
    user = centralComprobationUI(user);
    modifyUser(user, initial_dir);
    userFirstComprobationUI(user);
    userCheckKeysUI(user);
    
    cout << "###############Archivo de configuracion cargado##############"<< endl<<endl;
    upToDate(user);
    
    //while(true){
        //upToDate(user);
        //sleep(20);
    //}
        
    
    





    return 0;

}

