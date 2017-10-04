#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    create_tray_icon();
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
