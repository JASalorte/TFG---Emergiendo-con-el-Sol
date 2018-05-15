#include <winsock2.h>
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(QFileInfo(argv[0]).absoluteDir().absolutePath());
    MainWindow w;


    qDebug() << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    /*QString arq;

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
    }*/

    w.show();

    return a.exec();
}
