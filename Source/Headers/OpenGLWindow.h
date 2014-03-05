#ifndef __GUNSLINGERED_OPENGLWINDOW_H__
#define __GUNSLINGERED_OPENGLWINDOW_H__

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <vector>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class QOpenGLFramebufferObject;
class EditorViewport;

class OpenGLWindow : public QWindow
{
	Q_OBJECT
public:
	explicit OpenGLWindow( QWindow *p_pParent );
	~OpenGLWindow( );

	virtual void render( QPainter *p_pPainter );
	virtual void render( );

	virtual void initialize( );

	void setAnimating( bool p_Animating );

public slots:
	void renderLater( );
	void renderNow( );

protected:
	bool event( QEvent *p_pEvent );
	void exposeEvent( QExposeEvent *p_pEvent );
	void resizeEvent( QResizeEvent *p_pEvent );

private:
	bool m_UpdatePending;
	bool m_Animating;
	bool m_Initialised;
	bool m_ViewportsQueued;
	// Temporary for testing
	bool m_ViewportAdded;

	QOpenGLContext		*m_pGLContext;
	QOpenGLPaintDevice	*m_pGLPaintDevice;
	QOpenGLFunctions	*m_pGLFunctions;

	std::vector< EditorViewport* >	m_Viewport;
};

#endif // __GUNSLINGERED_OPENGLWINDOW_H__

