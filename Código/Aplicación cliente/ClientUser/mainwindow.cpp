using namespace std;

#include <QThread>

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <limits>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <regex>


#include <libssh/libssh.h>


#include "mainwindow.h"
#include "SSHFunctions.h"
#include "dateClass.h"
#include "dirClass.h"
#include "ui_mainwindow.h"
#include "filefunctions.h"
//#include "controller.h"

ssh_session my_ssh_session;
FileData currentConfig;
int rc;
Ui::MainWindow *uiT;


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
    return false;
}




vector <dirClass> list_recur(string directory, string filetype, vector<dirClass> &v) {
    DIR* dir;
    dirent* pdir;
    dirClass rdir;



    dir = opendir(directory.c_str());
    //qDebug() << directory.c_str();
    while ((pdir = readdir(dir))) {
        if (string(pdir->d_name).compare(".") && string(pdir->d_name).compare("..")) {
            //qDebug() << string(pdir->d_name).c_str();
            unsigned int pos = string(pdir->d_name).rfind("." + filetype);
            if (pos != std::string::npos && isValidDate(string(pdir->d_name).substr(0, pos))) {
                rdir.SetDate(string(pdir->d_name).substr(0, pos));
                rdir.SetDir(directory);
                rdir.SetFile(string(pdir->d_name));
                v.push_back(rdir);
            }

            /*struct stat s;
            stat(pdir->d_name, &s);
            if (s.st_mode & S_IFDIR) {
                list_recur(directory + "\\" + pdir->d_name, filetype, v);
            }*/
            //QFileInfo *file = new QFileInfo(QString::fromStdString(directory + "\\" + pdir->d_name));
            if(QFileInfo(QString::fromStdString(directory + "\\" + pdir->d_name)).isDir())
               list_recur(directory + "\\" + pdir->d_name, filetype, v);
        }
    }
    closedir(dir);

    return v;
}

vector<dirClass> filesSinceDate(string directory, string filetype, string date) {
    //dateClass datefind = dateClass(date);
    vector<dirClass> v, vSorted;
    list_recur(directory, filetype, v);



    for (unsigned int i = 0; i < v.size(); i++) {
        if (dateComparator(v.at(i), dirClass(date, "", ""))) {
            vSorted.push_back(v.at(i));
        }
    }

    qSort(vSorted.begin(), vSorted.end(), dateComparator);
    return vSorted;
}

int initConection(string host, string username, int port) {
    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL)
        return SSH_ERROR;
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, username.c_str());
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_SSH_DIR, "keys");
    return SSH_OK;
}

void changeState(string msg){
    uiT->lState->setText(msg.c_str());
}



int mainConnection(string host, string username, int port) {
    if (my_ssh_session == NULL)
        if (initConection(host, username, port) == SSH_ERROR){
            changeState("Imposible conectar");
            return SSH_ERROR;
        }

    //etiquetaControl(QString("Intentado conectarnos con el servidor."));
    //cout <<  << endl;
    // Connect to server
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK) {
        //fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(my_ssh_session));

        //etiquetaControl(QString("Nos hemos encontrado con un error al conectar: ").append(QString(ssh_get_error(my_ssh_session))));
        //cout << "Nos hemos encontrado con un error al conectar: " << ssh_get_error(my_ssh_session) << endl;
        changeState(string("Error: ").append(ssh_get_error(my_ssh_session)));
        ssh_free(my_ssh_session);
        return SSH_ERROR;
    }


    // Verify the server's identity
    // For the source code of verify_knowhost(), check previous example
    if (verify_knownhost(my_ssh_session) < 0) {
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return SSH_ERROR;
    }

    // Authenticate ourselves
    rc = authenticate_pubkey(my_ssh_session);
    if (rc != SSH_AUTH_SUCCESS) {
        /*fprintf(stderr, "Error authenticating with password: %s\n",
                ssh_get_error(my_ssh_session));*/
        changeState(string("Error: ").append(ssh_get_error(my_ssh_session)));

        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return SSH_ERROR;
    } else {
         //etiquetaControl(QString("Nos hemos conectado correctamente."));
        //cout << "Nos hemos conectado correctamente." << endl;
        changeState(string("Correcto"));
        return SSH_OK;
    }
}

void MainWindow::onProgressChagned(QString cadena){
    QLabel * label = findChild<QLabel*>("LabelState");
    label->setText(cadena);
}

class WorkerThread : public QThread {
    void run() {
        while(1) {
             controller();
        }
    }

    public:
    controller();
    // Define signal:
    signals:
    void progressChanged(QString info);
};

void refreshTime(){
    uiT->llastRefresh->setText(string(currentConfig.GetLastDate().toString()).c_str());
    uiT->lastRefreshHour->setText(string(currentConfig.GetLastRefresh()).c_str());
}

void refreshUI(){
    uiT->lconnectionInfo->setText(string(currentConfig.GetUser() + "@" + currentConfig.GetHost() + ":" + currentConfig.GetPortS()).c_str());
    uiT->EUser->setText(string (currentConfig.GetUser()).c_str());
    uiT->EHost->setText(string (currentConfig.GetHost()).c_str());
    uiT->EPort->setText(string (currentConfig.GetPortS()).c_str());

    uiT->lrefreshTime->setText(currentConfig.GetSecondsS().c_str());
    uiT->ERefresh->setText(currentConfig.GetSecondsSS().c_str());

    uiT->llastRefresh->setText(string(currentConfig.GetLastDate().toString()).c_str());
    uiT->lastRefreshHour->setText(string(currentConfig.GetLastRefresh()).c_str());

    uiT->EDir->setText(string(currentConfig.GetDir()).c_str());
    uiT->EFileType->setText(string(currentConfig.GetFileExtension()).c_str());
}

bool isValidRefresh(string n){
    /*regex num("^[5-999999]$");
      if (regex_match(n, num)) {
          return true;
      }
      return false;*/
    if (atoi(n) > 5)
        return true;
    else{
        false;
    }
}

FileData loadConfigUI(){
    FileData d;

    d.SetDir(uiT->EDir->text().toUtf8().constData());
    d.SetFileExtension(uiT->EFileType->text().toUtf8().constData());
    d.SetHost(uiT->EHost->text().toUtf8().constData());
    d.SetLastDate(dateClass(string(uiT->llastRefresh->text().toUtf8().constData())));
    qDebug() << string(uiT->llastRefresh->text().toUtf8().constData()).c_str();
    d.SetPort(atoi(uiT->EPort->text().toUtf8().constData()));
    if(isValidRefresh(uiT->EPort->text().toUtf8().constData()))
        d.SetSeconds(atoi(uiT->ERefresh->text().toUtf8().constData()));
    else
        d.SetSeconds(5);
    d.SetUser(uiT->EUser->text().toUtf8().constData());

    return d;

}

struct tm* getTime(){
    time_t t = time(0);
    return localtime(&t);
}

int WorkerThread::controller(){
    /*QString arq;

    arq="lock.ctl";
    QDir d("C:\\windows\\system32\\");
    d.remove(arq);
    return -1;*/

    /*#ifdef Q_OS_WIN32
        arq="C:\\windows\\system32\\lock.ctl";
    #else
        arq="/var/run/lock.ctl";
    #endif
    QFile dlock(arq);
    if(dlock.exists()) {
        //QMessageBox::critical(w,"MyApp", ".this programs already is running", QMessageBox::Close);
        QApplication::restoreOverrideCursor();
        //return -1;
    }
    else {
        dlock.open(QFile::WriteOnly | QFile::Truncate);
        QTextStream out(&dlock);
        out << "running" << endl;
        dlock.close();
    }*/

    /*string host = "150.214.174.39";
    int port = 10022;
    string username = "user0002";
    string lastDay = "2015-5-6";*/

    string lastDay = currentConfig.GetLastDate().toString();


    vector<dirClass> v = filesSinceDate(currentConfig.GetDir(), currentConfig.GetFileExtension(), lastDay);

    bool error;

    do {

            string host = currentConfig.GetHost();
            int port = currentConfig.GetPort();
            string username = currentConfig.GetUser();

            //delete my_ssh_session;
            my_ssh_session = NULL;
            error = mainConnection(host, username, port);

            //error = mainConnection(host, username);

            //qDebug("Acabo de enviar: %d", v.size());
        //qDebug("Numero de directorios: %d", v.size());
            while (v.size() > 1 && !error) {

                error = send_file(v.at(v.size() - 1).GetDir().c_str(), v.at(v.size() - 1).GetFile().c_str(), "workdir", my_ssh_session);
                if (!error) {

                    //etiquetaControl(QString("Acabo de enviar: ").append(QString(QString::fromStdString(v.at(v.size() - 1).GetFile()))));
                    //qDebug("Acabo de enviar: " +  string(v.at(v.size() - 1).GetFile()));
                    qDebug() << "Acabo de enviar: " << QString::fromStdString(v.at(v.size() - 1).GetFile());
                    lastDay = v.at(v.size() - 1).GetFile().substr(0, v.at(v.size() - 1).GetFile().rfind(".csv"));
                    currentConfig.SetLastDate(lastDay);
                    modifyConfig(currentConfig);
                    refreshTime();
                    if (v.size() != 1) {
                        v.pop_back();
                    }
                }
            }

            if (!error && v.size() != 0) {
                error = send_file(v.at(0).GetDir().c_str(), v.at(0).GetFile().c_str(), "workdir", my_ssh_session);
                if (!error) {
                    lastDay = v.at(0).GetFile().substr(0, v.at(0).GetFile().rfind(".csv"));
                    //etiquetaControl(QString("Actualizado: ").append(QString(QString::fromStdString(v.at(v.size() - 1).GetFile()))));
                    //cout << "Actualizado: " << v.at(v.size() - 1).GetFile() << endl;
                    qDebug() << "Actualizado: " << QString::fromStdString(v.at(v.size() - 1).GetFile());
                    //currentConfig.SetLastDate(lastDay);
                    struct tm* now = getTime();
                    string hour = static_cast<ostringstream*> (&(ostringstream() << (now->tm_hour)))->str();
                    string minute = static_cast<ostringstream*> (&(ostringstream() << (now->tm_min)))->str();
                    string second = static_cast<ostringstream*> (&(ostringstream() << (now->tm_sec)))->str();

                    if(hour.length() == 1)
                        hour = "0" + hour;
                    if(minute.length() == 1)
                        minute = "0" + minute;
                    if(second.length() == 1)
                        second = "0" + second;

                    if(dateClass(lastDay).toString() != currentConfig.GetLastDate().toString()){
                         currentConfig.SetLastDate(lastDay);
                         modifyConfig(currentConfig);
                         qDebug() << "Actualizado el fichero porque hemos cambiado de día";
                    }

                    currentConfig.SetLastRefresh(hour + ":" + minute + ":" + second);

                    //delete now;
                    refreshTime();
                }
            }
            if (error) {
                //etiquetaControl(QString("No se ha podido enviar nada, se volverá a intentar."));
                //cout << "No se ha podido enviar nada, se volverá a intentar." << endl;
                //changeState(string("Conexión perdida, reconectado..."));
            }else{
                ssh_disconnect(my_ssh_session);
                ssh_free(my_ssh_session);
            }

            //cout << v.size() << endl;

            sleep(currentConfig.GetSeconds());
            if (!error) {
                v = filesSinceDate(currentConfig.GetDir(), currentConfig.GetFileExtension(), lastDay);
            }

        } while (true);
}




void MainWindow::startWorkInAThread() {
    // Create an instance of your woker
    WorkerThread *workerThread = new WorkerThread;
    // Connect our signal and slot
    connect(workerThread, SIGNAL(progressChanged(QString)),
                          SLOT(onProgressChagned(QString)));
    // Setup callback for cleanup when it finishes
    connect(workerThread, SIGNAL(finished()),
            workerThread, SLOT(deleteLater()));
    // Run, Forest, run!
    workerThread->start(); // This invokes WorkerThread::run in a new thread
}





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    uiT = getUI();
    ui->setupUi(this);
    create_tray_icon();

    string path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).toUtf8().constData();
    path.append("/EmergiendoConElSol/Setup.conf");

    if (!OpenCppFileExists(path)){
        createConfigFile();
    }

    currentConfig = loadConfig();
    refreshUI();

    startWorkInAThread();

}



MainWindow::~MainWindow()
{
    QString arq;
    arq="lock.ctl";
    QDir d("C:\\windows\\system32\\");
    d.remove(arq);
    delete ui;
}

void MainWindow::changeEvent(QEvent* e)
{
    switch (e->type())
    {
        case QEvent::LanguageChange:
            this->ui->retranslateUi(this);
            break;
        case QEvent::WindowStateChange:
            {
                if (this->windowState() & Qt::WindowMinimized)
                {
                    //QTimer::singleShot(250, this, SLOT(hide()));
                    hide();
                    m_tray_icon->showMessage("Minimizada","Aplicación minimizada al area de notificación.", QSystemTrayIcon::Information,5000);
                }

                break;
            }
        default:
            break;
    }
}


void MainWindow::create_tray_icon()
 {
    m_tray_icon = new QSystemTrayIcon(QIcon("imageformats\\logo_ujaen.gif"), this);

    connect( m_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_show_hide(QSystemTrayIcon::ActivationReason)) );

    QAction *quit_action = new QAction( "Salir", m_tray_icon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

    QAction *hide_action = new QAction( "Mostrar/Ocultar", m_tray_icon );
    connect( hide_action, SIGNAL(triggered()), this, SLOT(on_show_hide()) );

    QMenu *tray_icon_menu = new QMenu;
    tray_icon_menu->addAction( hide_action );
    tray_icon_menu->addAction( quit_action );

    m_tray_icon->setContextMenu( tray_icon_menu );

    m_tray_icon->show();
}

void MainWindow::on_show_hide()
{
    if( isVisible() )
    {
        hide();
    }
    else
    {
        show();
        setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
        raise();
        activateWindow();

    }
}

void MainWindow::on_exit()
{
    QString arq;
    arq="lock.ctl";
    QDir d("C:\\windows\\system32\\");
    d.remove(arq);
    QApplication::quit();
}

void MainWindow::on_show_hide( QSystemTrayIcon::ActivationReason reason )
{
    if( reason )
    {
        if( reason != QSystemTrayIcon::DoubleClick )
        return;
    }

    on_show_hide();

}

void MainWindow::on_pushButton_clicked()
{
    currentConfig = loadConfigUI();
    modifyConfig(currentConfig);
    refreshUI();
}

void MainWindow::on_actionModify_triggered()
{

}
