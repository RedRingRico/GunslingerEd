#ifndef __GUNSLINGERED_EDITORVIEWPORTMANAGER_H__
#define __GUNSLINGERED_EDITORVIEWPORTMANAGER_H__

#include <vector>
#include <EditorViewport.h>

class EditorViewport;
class QOpenGLContext;

class EditorViewportManager
{
public:
	explicit EditorViewportManager( QOpenGLContext * const &p_pGLContext );
	~EditorViewportManager( );

	int Create( const ViewportType p_Type );

private:
	std::vector< EditorViewport * >	m_Viewports;
	QOpenGLContext					*m_pGLContext;
};

#endif // __GUNSLINGERED_EDITORVIEWPORTMANAGER_H__

