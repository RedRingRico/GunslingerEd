#include <EditorViewport.h>
#include <QtGui/QOpenGLFramebufferObject>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QFont>

EditorViewport::EditorViewport( QWidget *p_pParent ) :
	QWidget( p_pParent )
{
}

EditorViewport::~EditorViewport( )
{
}

int EditorViewport::Create( const ViewportType p_Type )
{
	m_pFramebuffer = new QOpenGLFramebufferObject( size( ), GL_TEXTURE_2D );

	printf( "Created with size: %dx%d\n", size( ).width( ),
		size( ).height( ) );

	m_Type = p_Type;

	return 0;
}

int EditorViewport::Resize( const int p_Width, const int p_Height )
{
	if( ( p_Width <= 0 ) || ( p_Height <= 0 ) )
	{
		return 1;
	}

	if( ( p_Width == m_Size.width( ) ) && ( p_Height == m_Size.height( ) ) )
	{
		return 1;
	}

	m_pFramebuffer->release( );
	delete m_pFramebuffer;
	m_pFramebuffer = nullptr;

	m_pFramebuffer = new QOpenGLFramebufferObject( p_Width, p_Height,
		GL_TEXTURE_2D );

	return 0;
}

int EditorViewport::SetType( const ViewportType p_Type )
{
	m_Type = p_Type;

	return 0;
}

int EditorViewport::SetClearColour( const float p_Red, const float p_Green,
	const float p_Blue )
{
	m_RedClear = p_Red;
	m_GreenClear = p_Green;
	m_BlueClear = p_Blue;

	return 0;
}

int EditorViewport::GetImage( QImage *p_pImage )
{
	( *p_pImage ) = m_pFramebuffer->toImage( );

	return 0;
}

void EditorViewport::Activate( )
{
	m_pFramebuffer->bind( );
}

void EditorViewport::Deactivate( )
{
	m_pFramebuffer->release( );
}

void EditorViewport::Render( )
{
	glClearColor( m_RedClear, m_GreenClear, m_BlueClear, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	printf( "Rendered\n" );
}

void EditorViewport::paintEvent( QPaintEvent *p_pPaintEvent )
{
	this->Activate( );
	this->SetClearColour( 0.12f, 0.12f, 0.2f );
	this->Render( );
	this->Deactivate( );
	printf( "Painting to %d,%d\n", pos( ).x( ), pos( ).y( ) );
	printf( "Framebuffer size: %dx%d\n", m_pFramebuffer->width( ),
		m_pFramebuffer->height( ) );

	QPainter Painter( this );
	QPoint Zero( 0, 0 );
	QRect Rectangle( Zero, size( ) );

	Painter.drawImage( Rectangle, m_pFramebuffer->toImage( ) );
	Painter.setPen( Qt::yellow );
	QFont Qfont = Painter.font( );
	Qfont.setPointSize( 20 );
	Painter.setFont( Qfont );
	Painter.drawText( QPoint( 0, pos( ).y( ) + 40 ), "Test" );
}

