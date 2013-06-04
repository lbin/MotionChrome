#ifndef OSGWIDGET_H
#define OSGWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QtGui/QKeyEvent>

#include <osgViewer/Viewer>

using Qt::WindowFlags;

class OSGWidget : public osgViewer::Viewer,public QGLWidget
{
public:
    OSGWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, WindowFlags f = 0 );

    virtual ~OSGWidget() {}

    osgViewer::GraphicsWindow* getGraphicsWindow()
    {
        return _gw.get();
    }
    const osgViewer::GraphicsWindow* getGraphicsWindow() const
    {
        return _gw.get();
    }

    QSize sizeHint() const
    {
        // 初始化窗口大小;
        return QSize(1200, 900);
    }
protected:
    void init();

    virtual void resizeGL( int width, int height );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
};


#endif // OSGWIDGET_H
