/* 
 * File:   main.cpp
 * Author: Linkku
 *
 * Created on 30 de enero de 2015, 11:49
 */

#include <mysql.h>

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

#include <sys/types.h>
#include <sys/inotify.h>



#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

using namespace std;

int main(int argc, char **argv) {

    vector<UserInfo> user;

    cout << "#################### Iniciando el servidor V0.1 ####################" << endl << endl;

    string initial_dir = "/var/www/html/JSON/Main.conf";


    if (!OpenCppFileExists(initial_dir)) {
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
    }

    string host = loadHost(initial_dir);
    string port = loadPort(initial_dir);

    cout << "#############Cargando archivo de configuración Main.conf.##################" << endl << endl;
    user = loadConfig(initial_dir);
    loadedUsersUI(user);
    user = centralComprobationUI(user);
    modifyUser(user, initial_dir);
    userFirstComprobationUI(user, host, port);

    cout << "###############Archivo de configuracion cargado##############" << endl << endl;

    cout << "Iniciando inotify" << endl;

    int length, i = 0;
    int fd;
    vector<int> wd;
    char buffer[BUF_LEN];
    int pos;

    fd = inotify_init();

    if (fd < 0) {
        perror("inotify_init");
    }

    wd.push_back(inotify_add_watch(fd, "/var/www/html/JSON", IN_CLOSE_WRITE));

    for (int x = 0; x < user.size(); x++)
        wd.push_back(inotify_add_watch(fd, string("/home/" + user.at(x).GetName() + "/workdir").c_str(), IN_CLOSE_WRITE));

    while (1) {
        struct inotify_event *event;

        length = read(fd, buffer, BUF_LEN);

        if (length < 0) {
            perror("read");
        }

        event = (struct inotify_event *) &buffer[ i ];

        MYSQL *conn;
        MYSQL_RES *res;
        MYSQL_ROW row;
        char *server = "localhost";
        char *userlogin = "";
        char *password = ""; 
        char *database = "";
        conn = mysql_init(NULL);
        /* Connect to database */
        if (!mysql_real_connect(conn, server,
                userlogin, password, database, 0, NULL, 0)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
        }

        if (event->len) {
            pos = find(wd.begin(), wd.end(), event->wd) - wd.begin();


            if (pos == 0) {
                if (string(event->name) == "Main.conf") {
                    cout << "#############Recargando archivo de configuración Main.conf.##################" << endl << endl;
                    user = loadConfig(initial_dir);
                    loadedUsersUI(user);
                    user = centralComprobationUI(user);
                    modifyUser(user, initial_dir);
                    userFirstComprobationUI(user, host, port);

                    cout << "###############Archivo de configuracion cargado##############" << endl << endl;
                    
                    wd.clear();

                    wd.push_back(inotify_add_watch(fd, "/var/www/html/JSON", IN_CLOSE_WRITE));

                    for (int x = 0; x < user.size(); x++)
                        wd.push_back(inotify_add_watch(fd, string("/home/" + user.at(x).GetName() + "/workdir").c_str(), IN_CLOSE_WRITE));
                }

            }


            if (pos > 0) {
                cout << "Ha cambiado el archivo " + string(event->name) + " del user: " + user.at(pos - 1).GetName() << endl;
                linux_return_function(string("sudo chmod 755 /home/" + user.at(pos - 1).GetName() + "/workdir/" + event->name).c_str());

                string query = "LOAD DATA LOCAL INFILE '/home/" + user.at(pos - 1).GetName() + "/workdir/" + event->name + "' "
                        "REPLACE "
                        "INTO TABLE data "
                        "FIELDS TERMINATED BY ';' ENCLOSED BY '' "
                        "LINES TERMINATED BY '\n' STARTING BY '' "
                        "IGNORE 1 LINES"
                        "(@d,@var1, @var2, @var3,@var4,@var5,@var6,@var7,@var8,@var9,@var10,@var11,@var12,@var13,@var14,@var15)"
                        "SET central = '" + user.at(pos - 1).GetCentral() + "', "
                        "date = UNIX_TIMESTAMP(CONVERT_TZ(STR_TO_DATE(@d, '%d/%m/%Y %H:%i:%s'), '+00:00', @@global.time_zone)),"
                        "var1 = REPLACE(@var1, ',', '.'),"
                        "var2 = REPLACE(@var2, ',', '.'),"
                        "var3 = REPLACE(@var3, ',', '.'),"
                        "var4 = REPLACE(@var4, ',', '.'),"
                        "var5 = REPLACE(@var5, ',', '.'),"
                        "var6 = REPLACE(@var6, ',', '.'),"
                        /*"var7 = REPLACE(@var7, ',', '.'),"
                        "var8 = REPLACE(@var8, ',', '.'),"
                        "var9 = REPLACE(@var9, ',', '.'),"*/
                        "var10 = REPLACE(@var10, ',', '.'),"
                        "var11 = REPLACE(@var11, ',', '.'),"
                        "var12 = REPLACE(@var12, ',', '.'),"
                        "var13 = REPLACE(@var13, ',', '.'),"
                        "var14 = REPLACE(@var14, ',', '.'),"
                        "var15 = REPLACE(@var15, ',', '.');";
                /* send SQL query */
                if (mysql_query(conn, query.c_str())) {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    //exit(1);
                }
            }
        }
        mysql_close(conn);
    }
    (void) inotify_rm_watch(fd, wd[0]);
    (void) close(fd);

    return 0;

}

