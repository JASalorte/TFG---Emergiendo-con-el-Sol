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
    QSystemTrayIcon *m_tray_icon;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void create_tray_icon();
    void changeEvent(QEvent* e);

public slots:
    void on_show_hide(QSystemTrayIcon::ActivationReason reason);
    void on_show_hide();
    void on_exit();

/*signals:
    on_show_hide_activated(QSystemTrayIcon::ActivationReason);
    on_show_hide_triggered();
    on_exit_triggered();*/


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
