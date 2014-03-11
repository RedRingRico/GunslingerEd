#include <EditorViewport.h>
#include <QtGui/QOpenGLFramebufferObject>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QFont>
#include <QOpenGLShaderProgram>
#include <QtGui/QMatrix4x4>
#include <QOpenGLFunctions>
#include <Utility.h>

static const char *pVertexShaderSource =
{
	"attribute highp vec4 a_Position;\n"
	"attribute lowp vec4 a_Colour;\n"
	"varying lowp vec4 f_Colour;\n"
	"uniform highp mat4 u_Matrix;\n"
	"void main( )\n"
	"{\n"
	"	f_Colour = a_Colour;\n"
	"	gl_Position = u_Matrix * a_Position;\n"
	"}\n"
};

static const char *pFragmentShaderSource =
{
	"varying lowp vec4 f_Colour;\n"
	"void main( )\n"
	"{\n"
	"	gl_FragColor = f_Colour;\n"
	"}\n"
};

EditorViewport::EditorViewport( QWidget *p_pParent ) :
	QWidget( p_pParent )
{
}

EditorViewport::~EditorViewport( )
{
}

int EditorViewport::Create( const ViewportType p_Type,
	QOpenGLFunctions * const &p_pGLFunctions )
{
	m_pGLFunctions = p_pGLFunctions;
	m_pFramebuffer = new QOpenGLFramebufferObject( size( ), GL_TEXTURE_2D );

	printf( "Created with size: %dx%d\n", size( ).width( ),
		size( ).height( ) );

	m_Type = p_Type;

	m_pProgram = new QOpenGLShaderProgram( this );
	m_pProgram->addShaderFromSourceCode( QOpenGLShader::Vertex,
		pVertexShaderSource );
	m_pProgram->addShaderFromSourceCode( QOpenGLShader::Fragment,
		pFragmentShaderSource );
	m_pProgram->link( );
	m_PositionAttribute = m_pProgram->attributeLocation( "a_Position" );
	m_ColourAttribute = m_pProgram->attributeLocation( "a_Colour" );
	m_MatrixUniform = m_pProgram->uniformLocation( "u_Matrix" );

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
	glViewport( 0, 0, width( ), height( ) );
	glClearColor( m_RedClear, m_GreenClear, m_BlueClear, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	m_pProgram->bind( );

	QMatrix4x4 Matrix;
	Matrix.perspective( 60,
		static_cast< float >( size( ).width( ) ) /
			static_cast< float >( size( ).height( ) ), 0.1f, 100.0f );
	Matrix.translate( 0, 0, -2 );

	m_pProgram->setUniformValue( m_MatrixUniform, Matrix );

	GLfloat Vertices[ ] =
	{
		0.0f, 0.707f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	GLfloat Colours[ ] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	m_pGLFunctions->glVertexAttribPointer( m_PositionAttribute, 2, GL_FLOAT,
		GL_FALSE, 0, Vertices );
	m_pGLFunctions->glVertexAttribPointer( m_ColourAttribute, 3, GL_FLOAT,
		GL_FALSE, 0, Colours );
	
	m_pGLFunctions->glEnableVertexAttribArray( 0 );
	m_pGLFunctions->glEnableVertexAttribArray( 1 );

	glDrawArrays( GL_TRIANGLES, 0, 3 );

	m_pGLFunctions->glDisableVertexAttribArray( 1 );
	m_pGLFunctions->glDisableVertexAttribArray( 0 );

	m_pProgram->release( );

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

void EditorViewport::resizeEvent( QResizeEvent *p_pResizeEvent )
{
	m_pFramebuffer->release( );
	SafeDelete( m_pFramebuffer );
	m_pFramebuffer = new QOpenGLFramebufferObject( size( ), GL_TEXTURE_2D );

	printf( "New framebuffer size: %dx%d\n", size( ).width( ),
		size( ).height( ) );
}

