#include <EditorViewport.h>
#include <QtGui/QOpenGLFramebufferObject>

EditorViewport::EditorViewport( )
{
}

EditorViewport::~EditorViewport( )
{
}

int EditorViewport::Create( const int p_Width, const int p_Height,
	const ViewportType p_Type )
{
	if( ( p_Width <= 0 ) || ( p_Height <= 0 ) )
	{
		return 1;
	}

	m_pFramebuffer = new QOpenGLFramebufferObject( p_Width, p_Height,
		GL_TEXTURE_2D );
	
	m_Size.setWidth( p_Width );
	m_Size.setHeight( p_Height );
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
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

