#ifndef horizonscanner_h
#define horizonscanner_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	N. Hemstra
 Date:		Feb 2004
 RCS:		$Id$
________________________________________________________________________

-*/

#include "earthmodelmod.h"
#include "executor.h"
#include "bufstringset.h"
#include "ranges.h"

class BinIDValueSet;
namespace EM { class Horizon3DAscIO; }
namespace Table { class FormatDesc; }
namespace PosInfo { class Detector; }

/*!
\brief Executor to scan horizons.
*/

mExpClass(EarthModel) HorizonScanner : public Executor
{ mODTextTranslationClass(HorizonScanner);
public:

			HorizonScanner(const BufferStringSet& fnms,
					Table::FormatDesc& fd, bool isgeom);
			~HorizonScanner();

    virtual uiString	uiMessage() const;
    virtual od_int64	totalNr() const;
    virtual od_int64	nrDone() const;
    virtual uiString	uiNrDoneText() const;

    bool		reInitAscIO(const char*);
    void		setPosIsXY(bool yn)		{ isxy_ = yn; }
    bool		posIsXY() const			{ return isxy_; }
    bool		analyzeData();

    int			nrPositions() const;
    StepInterval<int>	inlRg() const;
    StepInterval<int>	crlRg() const;
    bool		gapsFound(bool inl) const;

    static const char*	defaultUserInfoFile();
    void		launchBrowser(const char* fnm=0) const;
    void		report(IOPar&) const;

    const ObjectSet<BinIDValueSet>& getSections()	{ return sections_; }

protected:

    virtual int		nextStep();

    void		init();

    mutable int		totalnr_;
    int			nrdone_;
    PosInfo::Detector&	dtctor_;
    EM::Horizon3DAscIO*	ascio_;
    BufferStringSet	filenames_;
    int			fileidx_;
    BufferStringSet	rejectedlines_;

    bool		firsttime_;
    bool		isgeom_;
    bool		isxy_;
    bool		selxy_;
    bool		doscale_;
    TypeSet<Interval<float> > valranges_;
    Table::FormatDesc&	fd_;

    BinIDValueSet*	bvalset_;
    ObjectSet<BinIDValueSet> sections_;

    mutable uiString	curmsg_;
};


#endif
