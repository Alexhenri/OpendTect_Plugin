#ifndef ui3dindirectviewer_h
#define ui3dindirectviewer_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        K. Tingdahl
 Date:          Dec 2011
 RCS:           $Id$
________________________________________________________________________

-*/

#include "ui3dviewerbody.h"

class GraphicsWindowIndirect;

/*!
\brief Class used by ui3DViewer to render things indirectly.
*/

mClass(uiOSG) ui3DIndirectViewBody : public ui3DViewerBody
{
public:
				ui3DIndirectViewBody(ui3DViewer&,uiParent*);
				~ui3DIndirectViewBody();

    const QWidget*              qwidget_() const;

protected:
    osgViewer::GraphicsWindow&	getGraphicsWindow();
    osg::GraphicsContext*       getGraphicsContext();

						
    GraphicsWindowIndirect*	graphicswin_;    
};

#endif
