#ifndef view2dhorizon2d_h
#define view2dhorizon2d_h

/*+
________________________________________________________________________

 CopyRight:	(C) dGB Beheer B.V.
 Author:	Umesh Sinha
 Date:		May 2010
 RCS:		$Id$
________________________________________________________________________

-*/

#include "uiviewer2dmod.h"
#include "view2ddata.h"
#include "geom2dintersections.h"

#include "emposid.h"

class TrcKeyZSampling;
class uiFlatViewWin;
class uiFlatViewAuxDataEditor;

namespace Attrib { class SelSpec; }
namespace FlatView { class AuxDataEditor; }
namespace MPE { class HorizonFlatViewEditor2D; }


mExpClass(uiViewer2D) Vw2DHorizon2D : public Vw2DEMDataObject
{
public:
    static Vw2DHorizon2D* create(const EM::ObjectID& id,uiFlatViewWin* win,
			       const ObjectSet<uiFlatViewAuxDataEditor>& ed)
				mCreateVw2DDataObj(Vw2DHorizon2D,id,win,ed);

			~Vw2DHorizon2D();

    void		setSelSpec(const Attrib::SelSpec*,bool wva);
    void		setGeomID(Pos::GeomID);

    void		setTrcKeyZSampling(const TrcKeyZSampling&,
							    bool upd=false);

    void		draw();
    void		enablePainting(bool yn);
    void		selected(bool enabled=true);

    void		setSeedPicking(bool ison);
    void		setTrackerSetupActive(bool ison );
    void		setLine2DInterSectionSet(const Line2DInterSectionSet*
							ln2dintersectionset)
			{ line2dintersectionset_ = ln2dintersectionset; }
    const Line2DInterSectionSet* getLine2DInterSectionSet()
					    { return line2dintersectionset_; }

    void		getHorEditors(
			  ObjectSet<const MPE::HorizonFlatViewEditor2D>&) const;

    NotifierAccess*	deSelection()			{ return &deselted_; }

protected:

    void					triggerDeSel();
    void					setEditors();

    Pos::GeomID					geomid_;
    const Attrib::SelSpec*			vdselspec_;
    const Attrib::SelSpec*			wvaselspec_;

    ObjectSet<MPE::HorizonFlatViewEditor2D>	horeds_;
    Notifier<Vw2DHorizon2D>			deselted_;
    const Line2DInterSectionSet*		line2dintersectionset_;
};

#endif
