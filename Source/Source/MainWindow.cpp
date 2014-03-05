#include <MainWindow.h>
#include <QBoxLayout>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QSplitter>
#include <QContextMenuEvent>
#include <GitVersion.h>
#include <cstring>
#include <OpenGLWindow.h>

MainWindow::MainWindow( )
{
    QWidget *pWidget = new QWidget( );
    setCentralWidget( pWidget );

    QVBoxLayout *pLayout = new QVBoxLayout( );

    pLayout->setMargin( 5 );

	QSplitter *pSplit = new QSplitter( );

	m_pGLWindow = new OpenGLWindow( nullptr );
	QWidget *pGLWindow = QWidget::createWindowContainer( m_pGLWindow );
	pGLWindow->setFocusPolicy( Qt::TabFocus );
	pSplit->addWidget( pGLWindow );

	pLayout->addWidget( pSplit );

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
}

MainWindow::~MainWindow( )
{
	if( m_pGLWindow )
	{
		delete m_pGLWindow;
		m_pGLWindow = nullptr;
	}
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

