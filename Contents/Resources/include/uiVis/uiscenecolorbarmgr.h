#ifndef uiscenecolorbarmgr_h
#define uiscenecolorbarmgr_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        Ranojay Sen
 Date:          Oct 2009
 RCS:           $Id$
________________________________________________________________________

-*/

#include "uivismod.h"
#include "uidialog.h"

class uiGenInput;
class uiSpinBox;
namespace visBase{ class SceneColTab; }

mExpClass(uiVis) uiSceneColorbarMgr : public uiDialog
{ mODTextTranslationClass(uiSceneColorbarMgr);
public:
			uiSceneColorbarMgr(uiParent*,visBase::SceneColTab*);
			~uiSceneColorbarMgr();

protected:
    visBase::SceneColTab* scenecoltab_;
    uiSpinBox*		widthfld_;
    uiSpinBox*		heightfld_;
    uiGenInput*		posfld_;

    void		sizeChangedCB(CallBacker*);
    void		posChangedCB(CallBacker*);
};

#endif
