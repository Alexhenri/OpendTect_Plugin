#ifndef survinfo_h
#define survinfo_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	A.H.Bril
 Date:		9-4-1996
 RCS:		$Id$
________________________________________________________________________

-*/

#include "basicmod.h"
#include "namedobj.h"
#include "ranges.h"
#include "enums.h"
#include "zdomain.h"
#include "atomic.h"
#include "survgeom3d.h"

class ascostream;
class LatLong2Coord;
namespace Coords { class CoordSystem; }



/*!
\brief Holds survey general information.

  The surveyinfo is the primary source for ranges and steps.It also provides
  the transformation between inline/xline <-> coordinates and lat/long estimates

  Note: the Z range step is only a default. It should not be used further
  because different cubes/lines have different sample rates.

  The ranges are defined for two cubes: the entire survey, and a 'working area'.
  Normally, you'll want to have the working area.

  If you are an expert, and you feel you need more 'power', you may want to look
  at the bottom part of the class too for some more public functions.
*/

mExpClass(Basic) SurveyInfo : public NamedObject
{

    mGlobal(Basic) friend const SurveyInfo&	SI();


public:
			~SurveyInfo();
    bool		has2D() const;
    bool		has3D() const;

    StepInterval<int>	inlRange(bool work) const;
    StepInterval<int>	crlRange(bool work) const;
    const StepInterval<float>& zRange( bool work ) const;
    int			inlStep() const;
    int			crlStep() const;
    float		zStep() const;
    float		inlDistance() const; //!< distance for one increment
    float		crlDistance() const;
    float		getArea(const Interval<int>& inl,
			     const Interval<int>& crl) const;	//!<returns m2
    float		getArea(bool work) const ;		//!<returns m2

    Coord3		oneStepTranslation(const Coord3& planenormal) const;

    const TrcKeyZSampling&	sampling( bool work ) const
			{ return work ? wcs_ : tkzs_; }

    Coord		transform( const BinID& b ) const;
    BinID		transform(const Coord&) const;
			/*!<\note BinID will be snapped using work step. */

    bool		xyInFeet() const;
    const char*		getXYUnitString(bool withparens=true) const;
    uiString		getUiXYUnitString(bool abbrviated=true,
						    bool withparens=true) const;
    const ZDomain::Def&	zDomain() const;
    bool		depthsInFeet() const	{ return depthsinfeet_; }
    inline float	showZ2UserFactor() const
			{ return (float)zDomain().userFactor(); }

    bool		zIsTime() const;
			//!<Legacy, don't use. Use zDomain().isTime()
    inline bool		zInMeter() const
			{ return zDomain().isDepth() && !depthsinfeet_;}
			//!<Legacy, don't use
    inline bool		zInFeet() const
			{ return zDomain().isDepth() && depthsinfeet_;}
			//<Legacy, don't use
    const char*		getZUnitString(bool withparens=true) const
			//!<Legacy, don't use
			{ return zDomain().unitStr( withparens ); }
    const uiString	getUiZUnitString(bool withparens=true) const
			{ return zDomain().uiUnitStr( withparens ); }
    enum Unit		{ Second, Meter, Feet };
    Unit		xyUnit() const;
			//!<Legacy, don't use
    Unit		zUnit() const;
			//!<Legacy, don't use

    Coord		minCoord(bool work) const;
    Coord		maxCoord(bool work) const;
    bool		isInside(const BinID&,bool work) const;
    bool		isReasonable(const BinID&) const;
				//!< Checks if in or near survey
    bool		isReasonable(const Coord&) const;
				//!< Checks if in or near survey
    Interval<int>	reasonableRange(bool inl) const;
    int			maxNrTraces(bool work) const;

    void		checkInlRange(Interval<int>&,bool work) const;
			//!< Makes sure range is inside
    void		checkCrlRange(Interval<int>&,bool work) const;
			//!< Makes sure range is inside
    void		checkZRange(Interval<float>&,bool work) const;
			//!< Makes sure range is inside
    bool		includes(const BinID&,const float,bool work) const;
			//!< Returns true when pos is inside survey-range

    void		snap(BinID&,const BinID& dir=BinID(0,0)) const;
			//!< dir = 0 : auto; -1 round downward, 1 round upward
    void		snapStep(BinID&,const BinID& dir=BinID(0,0))const;
			//!< see snap() for direction
    void		snapZ(float&,int direction=0) const;
			//!< see snap() for direction

    float		seismicReferenceDatum() const	 {return seisrefdatum_;}
			/*!<In depth units (m or ft), positive upward
			    from sea level. Always in meters for time surveys */
    void		setSeismicReferenceDatum( float d ) { seisrefdatum_=d; }

    const IOPar&	pars() const			{ return pars_; }
    void		putZDomain(IOPar&) const;

    RefMan<Survey::Geometry3D> get3DGeometry(bool work) const;
    RefMan<Coords::CoordSystem>	getCoordSystem();
    ConstRefMan<Coords::CoordSystem> getCoordSystem() const;

    enum Pol2D	{ No2D=0, Both2DAnd3D=1, Only2D=2 };

    // Some public fns moved to bottom because they are rarely used; some fns
    // that have 'no user servicable parts inside' are at the very bottom

protected:

			SurveyInfo();

    BufferString	datadir_;
    BufferString	dirname_;

    ZDomain::Def&	zdef_;
    bool		xyinfeet_;
    bool		depthsinfeet_;
    TrcKeyZSampling&	tkzs_;
    TrcKeyZSampling&	wcs_;
    float		seisrefdatum_;
    IOPar&		pars_;
    RefMan<Coords::CoordSystem> coordsystem_;

    mutable Threads::AtomicPointer<Survey::Geometry3D>	s3dgeom_;
    mutable Threads::AtomicPointer<Survey::Geometry3D>	work_s3dgeom_;

    Pos::IdxPair2Coord	b2c_;
    LatLong2Coord&	ll2c_;
    BinID		set3binids_[3];
    Coord		set3coords_[3];

    Pol2D		survdatatype_;
    bool		survdatatypeknown_;

    BufferString	comment_;
    BufferString	sipnm_;

    void		handleLineRead(const BufferString&,const char*);
    bool		wrapUpRead();
    void		writeSpecLines(ascostream&) const;

    void		setTr(Pos::IdxPair2Coord::DirTransform&,const char*);
    void		putTr(const Pos::IdxPair2Coord::DirTransform&,
				ascostream&,const char*) const;

private:

    // ugly, but hard to avoid:
    friend class		IOMan;
    friend class		uiSurvey;
    friend class		uiSurveyMap;
    friend class		uiSurveyInfoEditor;

    Pos::IdxPair2Coord::DirTransform rdxtr_;
    Pos::IdxPair2Coord::DirTransform rdytr_;

				// For IOMan only
    static void			setSurveyName(const char*);
				// friends only
    static const char*		surveyFileName();

public:

	// These fns are rarely used by non-specialist classes.

    void		setWorkRange(const TrcKeyZSampling&);
    Notifier<SurveyInfo> workRangeChg;

    const Pos::IdxPair2Coord&	binID2Coord() const	{ return b2c_; }
    void		get3Pts(Coord c[3],BinID b[2],int& xline) const;
    const LatLong2Coord& latlong2Coord() const	{ return ll2c_; }
    bool		isClockWise() const { return isRightHandSystem(); }
			//!<Don't use. Will be removed
    bool		isRightHandSystem() const;
			/*!< Orientation is determined by rotating the
			     inline axis to the crossline axis. */
    float		angleXInl() const;
			/*!< It's the angle between the X-axis (East) and
			     an Inline */
    float		angleXCrl() const;
			/*!< It's the angle between the X-axis (East) and
			     a Crossline */
    void		setXYInFeet( bool yn=true ) { xyinfeet_ = yn; }
    void		setDepthInFeet( bool yn=true ) { depthsinfeet_ = yn; }
    void		setZUnit(bool istime,bool infeet=false);
    static float	defaultXYtoZScale(Unit,Unit);
			/*!<Gives a ballpark figure of how to scale XY to
			    make it comparable to Z. */
    float		zScale() const;
			/*!<Gives a ballpark figure of how to scale Z to
			    make it comparable to XY. */

    static const char*	sKeyInlRange();
    static const char*	sKeyCrlRange();
    static const char*	sKeyXRange();
    static const char*	sKeyYRange();
    static const char*	sKeyZRange();
    static const char*	sKeyXYInFt();
    static const char*	sKeyDpthInFt(); //!< Not used by SI, just a UI default
    static const char*	sKeySurvDataType();
    static const char*  sKeySeismicRefDatum();
    static const char*	sKeySetupFileName()		{ return ".survey"; }
    static const char*	sKeyBasicSurveyName()		{ return "BasicSurvey";}

    BufferString	getDirName() const	{ return dirname_; }
    BufferString	getDataDirName() const	{ return datadir_; }
    void		updateDirName(); //!< May be used after setName()

			mDeclareEnumUtils(Pol2D);
    Pol2D		survDataType() const	{ return survdatatype_; }
    void		setSurvDataType( Pol2D typ )
			{ survdatatype_ = typ; survdatatypeknown_ = true; }

			// Auxiliary info
    const char*		comment() const		{ return comment_.buf(); }
    BufferString	sipName() const		{ return sipnm_; }
    void		setSipName( BufferString sipnm )     { sipnm_ = sipnm; }
    void		setComment( const char* s )	{ comment_ = s; }

	// Following fns are used by specialist classes. Don't use casually.

			SurveyInfo(const SurveyInfo&);
    SurveyInfo&		operator =(const SurveyInfo&);

    Pos::IdxPair2Coord&	getBinID2Coord() const
			{ return const_cast<SurveyInfo*>(this)->b2c_; }
    LatLong2Coord&	getLatlong2Coord() const
			{ return const_cast<SurveyInfo*>(this)->ll2c_; }
    IOPar&		getPars() const
			{ return const_cast<SurveyInfo*>(this)->pars_; }

    bool		write(const char* basedir=0) const;
			//!< Write to .survey file
    void		savePars(const char* basedir=0) const;
			//!< Write to .defs file
    static SurveyInfo*	read(const char* survdir);
    static SurveyInfo*	read(const char* path,bool pathisfile);
    void		setRange(const TrcKeyZSampling&,bool);
    const char*		set3Pts(const Coord c[3],const BinID b[2],int xline);
    void		gen3Pts();
    bool		setCoordSystem(Coords::CoordSystem*);
    void		readSavedCoordSystem() const;
			//!< Useful after loading plugins.

    void		update3DGeometry();

    static const char*	curSurveyName();

			// No, you really don't need these!
    static void		pushSI(SurveyInfo*);
    static SurveyInfo*	popSI();
    static void		deleteInstance()		{ delete popSI(); }

};


mGlobal(Basic) const SurveyInfo& SI();
mGlobal(Basic) inline SurveyInfo& eSI()
			{ return const_cast<SurveyInfo&>(SI()); }

mExternC( Basic ) const char* GetSurveyName(void);



#endif
