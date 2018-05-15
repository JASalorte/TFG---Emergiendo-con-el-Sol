#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QMouseEvent>
#include <QPixmap>
#include <QAction>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QApplication>
#include <QTimerEvent>



namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
   public:
    //Controller wor;

public:
    QSystemTrayIcon *m_tray_icon;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void create_tray_icon();
    void changeEvent(QEvent* e);
    //void etiquetaControl(QString cadena);
    void startWorkInAThread();
    Ui::MainWindow * getUI(){
        return ui;
    }

    //int controller();
    //int mainConnection(std::string host, std::string username, int port);

public slots:
    void onProgressChagned(QString info);
    void on_show_hide(QSystemTrayIcon::ActivationReason reason);
    void on_show_hide();
    void on_exit();

/*signals:
    on_show_hide_activated(QSystemTrayIcon::ActivationReason);
    on_show_hide_triggered();
    on_exit_triggered();*/


private slots:
    void on_actionModify_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
