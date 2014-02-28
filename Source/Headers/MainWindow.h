#ifndef GUNSLINGERED_MAINWINDOW_H
#define GUNSLINGERED_MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( );
    ~MainWindow( );

private:
    void CreateActions( );
    void CreateMenus( );

    QMenu	*m_pFileMenu;
    QAction	*m_pQuitAction;
};

#endif // GUNSLINGERED_MAINWINDOW_H
