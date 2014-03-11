#include <MainWindow.h>
#include <QBoxLayout>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QSplitter>
#include <QContextMenuEvent>
#include <GitVersion.h>
#include <cstring>
#include <QtGui/QOpenGLContext>
#include <QOffscreenSurface>
#include <Utility.h>
#include <EditorViewportManager.h>
#include <EditorViewport.h>
#include <QOpenGLFunctions>

MainWindow::MainWindow( )
{

}

MainWindow::~MainWindow( )
{
	SafeDelete( m_pOpenGLSurface );
	SafeDelete( m_pSplitter );
}

int MainWindow::Initialise( )
{
	m_pOpenGLSurface = new QOffscreenSurface( );
	m_pOpenGLSurface->create( );

	m_pGLContext = new QOpenGLContext( );
	m_pGLContext->create( );
	m_pGLContext->makeCurrent( m_pOpenGLSurface );

	m_pOpenGLFunctions = new QOpenGLFunctions( m_pGLContext );
	m_pOpenGLFunctions->initializeOpenGLFunctions( );

	if( m_pGLContext->isValid( ) == false )
	{
		return 1;
	}
    QWidget *pWidget = new QWidget( );
    setCentralWidget( pWidget );

    QVBoxLayout *pLayout = new QVBoxLayout( );

    pLayout->setMargin( 5 );

	m_pSplitter = new QSplitter( );

	this->Initialise3DSplitter( );

	pLayout->addWidget( m_pSplitter );

    pWidget->setLayout( pLayout );

    this->CreateActions( );
    this->CreateMenus( );

    QString Message = tr( "Waiting..." );
    statusBar( )->showMessage( Message );

	char Title[ 1000 ];
	memset( Title, '\0', sizeof( Title ) );
	sprintf( Title, "Gunslinger Editor [%s] %s //Build date: %s%s",
		GIT_BUILD_VERSION, GIT_COMMITHASH, GIT_COMMITTERDATE,
#if defined BUILD_DEBUG
		" [DEBUG]"
#else
		""
#endif // BUILD_DEBUG
		);
    setWindowTitle( tr( Title ) );
    setMinimumSize( 640, 480 );
    resize( 1024, 768 );


	return 0;
}

void MainWindow::CreateActions( )
{
    m_pQuitAction = new QAction( tr( "&Quit" ), this );
    m_pQuitAction->setShortcuts( QKeySequence::Quit );
    connect( m_pQuitAction, SIGNAL( triggered( ) ), this, SLOT( close( ) ) );
}

void MainWindow::CreateMenus( )
{
    m_pFileMenu = menuBar( )->addMenu( tr( "&File" ) );
    m_pFileMenu->addAction( m_pQuitAction );
}

void MainWindow::Initialise3DSplitter( )
{
	m_pViewportManager = new EditorViewportManager( m_pGLContext );
	EditorViewport *pTmpViewport = new EditorViewport( );

	EditorViewport *pTmpViewport2 = new EditorViewport( );

	m_pSplitter->addWidget( pTmpViewport );
	m_pSplitter->addWidget( pTmpViewport2 );

	pTmpViewport->Create( ViewportPerspective, m_pOpenGLFunctions );
	pTmpViewport2->Create( ViewportOrthographic, m_pOpenGLFunctions );
}

