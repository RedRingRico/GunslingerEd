#ifndef GUNSLINGERED_MAINWINDOW_H
#define GUNSLINGERED_MAINWINDOW_H

#include <QMainWindow>

class QOpenGLContext;
class QOffscreenSurface;
class QSplitter;
class EditorViewportManager;
class QOpenGLFunctions;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( );
    ~MainWindow( );

	int Initialise( );

private:
    void CreateActions( );
    void CreateMenus( );
	void Initialise3DSplitter( );

    QMenu					*m_pFileMenu;
    QAction					*m_pQuitAction;
	QOpenGLContext			*m_pGLContext;
	QOffscreenSurface		*m_pOpenGLSurface;
	QOpenGLFunctions		*m_pOpenGLFunctions;
	QSplitter				*m_pSplitter;
	EditorViewportManager	*m_pViewportManager;
};

#endif // GUNSLINGERED_MAINWINDOW_H

