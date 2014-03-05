#include <OpenGLWindow.h>
#include <QtCore/QCoreApplication>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

OpenGLWindow::OpenGLWindow( QWindow *p_pParent ) :
	QWindow( p_pParent ),
	m_UpdatePending( false ),
	m_Animating( false ),
	m_pGLContext( nullptr ),
	m_pGLPaintDevice( nullptr )
{
	setSurfaceType( QWindow::OpenGLSurface );
}

OpenGLWindow::~OpenGLWindow( )
{
	if( m_pGLPaintDevice )
	{
		delete m_pGLPaintDevice;
		m_pGLPaintDevice = nullptr;
	}

	if( m_pGLFunctions )
	{
		delete m_pGLFunctions;
		m_pGLFunctions = nullptr;
	}

	if( m_pGLContext )
	{
		delete m_pGLContext;
		m_pGLContext = nullptr;
	}
}

void OpenGLWindow::render( QPainter *p_pPainter )
{
	Q_UNUSED( p_pPainter );
}

void OpenGLWindow::render( )
{
	if( !m_pGLPaintDevice )
	{
		m_pGLPaintDevice = new QOpenGLPaintDevice;
	}

	glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );

	glClear( GL_COLOR_BUFFER_BIT | 	GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT );

	m_pGLPaintDevice->setSize( size( ) );

	QPainter Painter( m_pGLPaintDevice );
	render( &Painter );
}

void OpenGLWindow::initialize( )
{
}

void OpenGLWindow::setAnimating( bool p_Animating )
{
	m_Animating = p_Animating;
	
	if( m_Animating )
	{
		this->renderLater( );
	}
}

void OpenGLWindow::renderLater( )
{
	if( !m_UpdatePending )
	{
		m_UpdatePending = true;
		QCoreApplication::postEvent( this,
			new QEvent( QEvent::UpdateRequest ) );
	}
}

void OpenGLWindow::renderNow( )
{
	if( !isExposed( ) )
	{
		return;
	}

	m_UpdatePending = false;

	bool NeedsInitialisation = false;

	if( !m_pGLContext )
	{
		m_pGLContext = new QOpenGLContext( this );
		m_pGLContext->setFormat( requestedFormat( ) );
		m_pGLContext->create( );

		NeedsInitialisation = true;
	}

	m_pGLContext->makeCurrent( this );

	if( NeedsInitialisation )
	{
		m_pGLFunctions = new QOpenGLFunctions( m_pGLContext );
		m_pGLFunctions->initializeOpenGLFunctions( );
		initialize( );
	}

	render( );

	m_pGLContext->swapBuffers( this );

	if( m_Animating )
	{
		renderLater( );
	}
}

bool OpenGLWindow::event( QEvent *p_pEvent )
{
	switch( p_pEvent->type( ) )
	{
		case QEvent::UpdateRequest:
		{
			this->renderNow( );
			return true;
		}
		default:
		{
			return QWindow::event( p_pEvent );
		}
	}
}

void OpenGLWindow::exposeEvent( QExposeEvent *p_pEvent )
{
	Q_UNUSED( p_pEvent );

	if( isExposed( ) )
	{
		this->renderNow( );
	}
}

void OpenGLWindow::resizeEvent( QResizeEvent *p_pEvent )
{
	Q_UNUSED( p_pEvent );

	if( isExposed( ) )
	{
		this->renderNow( );
	}
}
