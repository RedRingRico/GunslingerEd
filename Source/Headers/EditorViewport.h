#ifndef __GUNSLINGERED_EDITORVIEWPORT_H__
#define __GUNSLINGERED_EDITORVIEWPORT_H__

#include <QColor>
#include <QImage>
#include <QSize>

class QOpenGLFramebufferObject;

typedef enum __ViewportType
{
	ViewportOrthographic,
	ViewportPerspective
}ViewportType;

class EditorViewport
{
public:
	EditorViewport( );
	~EditorViewport( );

	int Create( const int p_Width, const int p_Height,
		const ViewportType p_Type );
	int Resize( const int p_Width, const int p_Height );

	int SetType( const ViewportType p_Type );
	int SetClearColour( const float p_Red, const float p_Green,
		const float p_Blue );

	int GetImage( QImage *p_pImage );

	void Activate( );
	void Deactivate( );

	void Render( );

private:
	QOpenGLFramebufferObject	*m_pFramebuffer;
	QSize						m_Size;
	ViewportType				m_Type;
	float						m_RedClear;
	float						m_GreenClear;
	float						m_BlueClear;
};

#endif // __GUNSLINGERED_EDITORVIEWPORT_H__

