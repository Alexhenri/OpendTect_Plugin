/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        Nanne Hemstra
 Date:          June 2001
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "uisurvinfoed.h"
#include "uisip.h"

#include "bufstringset.h"
#include "coordsystem.h"
#include "trckeyzsampling.h"
#include "file.h"
#include "filepath.h"
#include "hiddenparam.h"
#include "ioman.h"
#include "iopar.h"
#include "mousecursor.h"
#include "oddirs.h"
#include "ptrman.h"
#include "statrand.h"
#include "survinfo.h"
#include "systeminfo.h"
#include "settings.h"
#include "unitofmeasure.h"
#include "latlong.h"

#include "uibutton.h"
#include "uicombobox.h"
#include "uifiledlg.h"
#include "uifileinput.h"
#include "uigeninput.h"
#include "uigroup.h"
#include "uilabel.h"
#include "uilatlong2coord.h"
#include "uilistbox.h"
#include "uimsg.h"
#include "uiseparator.h"
#include "uisurvey.h"
#include "uisurveyselect.h"
#include "od_helpids.h"

#include "qwidget.h"


extern "C" const char* GetBaseDataDir();

uiString uiSurveyInfoEditor::getSRDString( bool infeet )
{
    uiString lbl = uiString( tr("%1%2%3") )
		 .arg( SurveyInfo::sKeySeismicRefDatum() )
		 .arg( infeet ? "  " : " " ) //to keep string always same length
		 .arg( getDistUnitString(infeet,true) );

    return lbl;
}


uiString uiSurveyInfoEditor::getCoordString( bool infeet )
{
    uiString txt = tr("Coordinates are in %1%2")
			.arg( getDistUnitString(infeet,false)).arg("  ");
    return txt;
}


uiSurveyInfoEditor::uiSurveyInfoEditor( uiParent* p, SurveyInfo& si,
					bool isnew )
	: uiDialog(p,uiDialog::Setup(tr("Edit Survey Parameters"),
				     mNoDlgTitle,
				     mODHelpKey(mSurveyInfoEditorHelpID))
					.nrstatusflds(1))
	, rootdir_(GetBaseDataDir())
	, orgdirname_(si_.dirname_.buf())
	, si_(si)
	, survParChanged(this)
	, x0fld_(0)
	, dirnamechanged(false)
	, sipfld_(0)
	, lastsip_(0)
	, impiop_(0)
	, topgrp_(0)
	, xyinftfld_(0)
	, isnew_(isnew)
	, coordsystem_(si.getCoordSystem())
{
    orgstorepath_ = si_.datadir_.buf();

    BufferString fulldirpath;
    if ( isnew_ )
    {
	fulldirpath = FilePath( rootdir_ ).add( orgdirname_ ).fullPath();
	SurveyInfo::pushSI( &si_ );
    }
    else
    {
	BufferString storagedir = FilePath(orgstorepath_).add(orgdirname_)
							 .fullPath();
	int linkcount = 0;
	while ( linkcount++ < 20 && File::isLink(storagedir) )
	{
	    BufferString newstoragedir = File::linkTarget(storagedir);
	    FilePath fp( newstoragedir );
	    if ( !fp.isAbsolute() )
	    {
		fp.setPath( FilePath(storagedir).pathOnly() );
		newstoragedir = fp.fullPath();
	    }
	    storagedir = newstoragedir;
	}
	if ( linkcount < 20 )
	{
	    FilePath fp( storagedir );
	    orgstorepath_ = fp.pathOnly();
	    orgdirname_ = fp.fileName();
	}

	fulldirpath = storagedir;
    }

    topgrp_ = new uiGroup( this, "Top group" );
    survnmfld_ = new uiGenInput( topgrp_, tr("Survey name"),
				StringInpSpec(si_.name()) );
    survnmfld_->setElemSzPol( uiObject::Wide );

    pathfld_ = new uiGenInput( topgrp_, tr("Location on disk"),
				StringInpSpec(orgstorepath_) );
    pathfld_->attach( alignedBelow, survnmfld_ );
    pathfld_->setElemSzPol( uiObject::Wide );

#ifdef __win__
    pathfld_->setSensitive( false );
#else
    uiButton* pathbut = uiButton::getStd( topgrp_, OD::Select,
			      mCB(this,uiSurveyInfoEditor,pathbutPush), false );
    pathbut->attach( rightOf, pathfld_ );
#endif

    uiLabeledComboBox* lcb = new uiLabeledComboBox( topgrp_,
				SurveyInfo::Pol2DNames(), tr("Survey type") );
    lcb->attach( alignedBelow, pathfld_ ); pol2dfld_ = lcb->box();

    mkSIPFld( lcb->attachObj() );
    if ( sipfld_ )
	topgrp_->setHAlignObj( sipfld_ );
    else
	topgrp_->setHAlignObj( lcb );

    uiSeparator* horsep1 = new uiSeparator( this, "Hor sep 1" );
    horsep1->attach( stretchedBelow, topgrp_, -2 );

    uiLabel* rglbl = new uiLabel( this, tr("Survey ranges:") );
    rglbl->attach( leftBorder );
    rglbl->attach( ensureBelow, horsep1 );
    mkRangeGrp();
    rangegrp_->attach( alignedBelow, topgrp_ );
    rangegrp_->attach( ensureBelow, horsep1 );

    uiSeparator* horsep2 = new uiSeparator( this, "Hor sep 2" );
    horsep2->attach( stretchedBelow, rangegrp_ );

    uiLabel* crdlbl = new uiLabel( this, tr("Coordinate settings:") );
    crdlbl->attach( leftBorder );
    crdlbl->attach( ensureBelow, horsep2 );
    coordset = new uiGenInput( this, uiStrings::sEmptyString(),
			 BoolInpSpec(true,tr("Easy"), uiStrings::sAdvanced()) );
    coordset->attach( alignedBelow, rangegrp_ );
    coordset->attach( rightTo, crdlbl );
    coordset->valuechanged.notify( mCB(this,uiSurveyInfoEditor,chgSetMode));

    mkCoordGrp();
    crdgrp_->attach( alignedBelow, rangegrp_ );
    crdgrp_->attach( ensureBelow, coordset );

    mkTransfGrp();
    trgrp_->attach( alignedBelow, rangegrp_ );
    trgrp_->attach( ensureBelow, coordset );

    uiButton* applybut = uiButton::getStd( this, OD::Apply,
			    mCB(this,uiSurveyInfoEditor,appButPushed), true );
    applybut->attach( alignedBelow, crdgrp_ );

    coordsysfld_ = new uiPushButton( this, tr("Define Coordinate System"),
			mCB(this,uiSurveyInfoEditor,coordSystemCB), false );
    coordsysfld_->attach( rightTo, coordset );
    coordsysfld_->attach( rightBorder );

    xyunitlbl_ = new uiLabel( this, getCoordString(xyInFeet()) );
    xyunitlbl_->attach( rightTo, applybut );
    xyunitlbl_->attach( rightBorder );

    postFinalise().notify( mCB(this,uiSurveyInfoEditor,doFinalise) );
}


uiSurveyInfoEditor::~uiSurveyInfoEditor()
{
    delete impiop_;
    if ( isnew_ )
	SurveyInfo::popSI();
}


void uiSurveyInfoEditor::mkSIPFld( uiObject* att )
{
    sips_ = survInfoProvs();
    for ( int idx=0; idx<sips_.size(); idx++ )
    {
	if ( !sips_[idx]->isAvailable() )
	    { sips_.removeSingle(idx); idx--; }
    }
    const int nrprovs = sips_.size();
    if ( nrprovs < 1 ) return;

    uiLabeledComboBox* lcb = new uiLabeledComboBox( topgrp_,
					    tr("Ranges/coordinate settings") );
    lcb->attach( alignedBelow, att );
    sipfld_ = lcb->box();
    sipfld_->addItem( tr("Enter below") );
    sipfld_->selectionChanged.notify( mCB(this,uiSurveyInfoEditor,sipCB) );
    for ( int idx=0; idx<nrprovs; idx++ )
    {
	uiSurvInfoProvider& sip = *sips_[idx];
	sipfld_->addItem( mToUiStringTodo(sip.usrText()) );
	const char* icnm = sip.iconName();
	if ( icnm && *icnm )
	    sipfld_->setIcon( sipfld_->size()-1, icnm );
    }
    sipfld_->setCurrentItem( 0 );

    if ( !si_.sipName().isEmpty() )
    {
	const BufferString sipnm = si_.sipName();
	const int sipidx = sipfld_->indexOf( sipnm );
	if (sipidx >= 0)
	    sipfld_->setCurrentItem(sipidx);
	else
	    uiMSG().error( tr("The survey setup method is not available.\n"
			      "Probably, this is a license issue") );
    }
}


void uiSurveyInfoEditor::mkRangeGrp()
{
    rangegrp_ = new uiGroup( this, "Survey ranges" );

    const Interval<int> startstoprg( -mUdf(int), mUdf(int) );
    const Interval<int> steprg( 1, mUdf(int) );
    IntInpIntervalSpec iis( true );
    iis.setLimits( startstoprg, -1 ).setLimits( steprg, 2 );
    iis.setName("Inl Start",0).setName("Inl Stop",1).setName("Inl step",2);
    inlfld_ = new uiGenInput( rangegrp_,
			      uiStrings::phrInline(uiStrings::sRange()), iis );
    inlfld_->valuechanged.notify( mCB(this,uiSurveyInfoEditor,rangeChg) );

    iis.setName("Crl Start",0).setName("Crl Stop",1).setName("Crl step",2);
    crlfld_ = new uiGenInput( rangegrp_, tr("Cross-line range"), iis );
    crlfld_->valuechanged.notify( mCB(this,uiSurveyInfoEditor,rangeChg) );

    zfld_ = new uiGenInput( rangegrp_, tr("Z range"),
			   DoubleInpIntervalSpec(true).setName("Z Start",0)
						      .setName("Z Stop",1)
						      .setName("Z step",2) );
    zfld_->valuechanged.notify( mCB(this,uiSurveyInfoEditor,rangeChg) );
    crlfld_->attach( alignedBelow, inlfld_ );
    zfld_->attach( alignedBelow, crlfld_ );

    const bool zistime = si_.zDomain().isTime();
    const bool depthinft = si_.depthsInFeet();

    const char* zunitstrs[] = { "millisecond", "meter", "feet", 0 };
    zunitfld_ = new uiComboBox( rangegrp_, zunitstrs, "Z unit" );
    zunitfld_->attach( rightOf, zfld_ );
    zunitfld_->setCurrentItem( zistime ? 0 : depthinft ? 2 : 1 );
    zunitfld_->selectionChanged.notify( mCB(this,uiSurveyInfoEditor,updZUnit));

    depthdispfld_ = new uiGenInput( rangegrp_, tr("Display depths in"),
		BoolInpSpec(!depthinft,tr("meter"),tr("feet")) );
    depthdispfld_->setSensitive( zistime && !si_.xyInFeet() );
    depthdispfld_->valuechanged.notify(
			mCB(this,uiSurveyInfoEditor,depthDisplayUnitSel) );
    depthdispfld_->attach( alignedBelow, zfld_ );

    const float srd = si_.seismicReferenceDatum();
    const UnitOfMeasure* datauom = zistime || !depthinft
				 ? UoMR().get( "Meter" ) : UoMR().get( "Feet" );
    const UnitOfMeasure* displayuom = depthdispfld_->getBoolValue()
				    ? UoMR().get( "Meter" )
				    : UoMR().get( "Feet" );
    refdatumfld_ = new uiGenInput( rangegrp_, getSRDString(depthinft),
		       FloatInpSpec(getConvertedValue(srd,datauom,displayuom)));
    refdatumfld_->attach( alignedBelow, depthdispfld_ );

    rangegrp_->setHAlignObj( inlfld_ );
}


void uiSurveyInfoEditor::mkCoordGrp()
{
    crdgrp_ = new uiGroup( this, "Coordinate settings" );
    PositionInpSpec::Setup psetup;
    ic0fld_ = new uiGenInput( crdgrp_, tr("First In-line/Cross-line"),
		     PositionInpSpec(psetup).setName("Inl Position1",0)
					    .setName("Crl Position1",1) );
    ic0fld_->valuechanging.notify( mCB(this,uiSurveyInfoEditor,setInl1Fld) );
    ic1fld_ = new uiGenInput( crdgrp_, tr("Another position on above In-line"),
		     PositionInpSpec(psetup).setName("Inl Position2",0)
					    .setName("Crl Position2",1) );
    ic2fld_ = new uiGenInput( crdgrp_, tr("Position not on above In-line"),
		      PositionInpSpec(psetup).setName("Inl Position3",0)
					     .setName("Crl Position3",1) );
    psetup.wantcoords_ = true;
    uiString xystr = tr("= (X,Y)");
    xy0fld_ = new uiGenInput( crdgrp_, xystr,
				PositionInpSpec(psetup).setName("X1",0)
						       .setName("Y1",1) );
    xy0fld_->setElemSzPol( uiObject::SmallVar );
    xy1fld_ = new uiGenInput( crdgrp_, xystr,
				PositionInpSpec(psetup).setName("X2",0)
						       .setName("Y2",1) );
    xy1fld_->setElemSzPol( uiObject::SmallVar );
    xy2fld_ = new uiGenInput( crdgrp_, xystr,
				PositionInpSpec(psetup).setName("X3",0)
						       .setName("Y3",1) );
    xy2fld_->setElemSzPol( uiObject::SmallVar );
    ic1fld_->attach( alignedBelow, ic0fld_ );
    ic2fld_->attach( alignedBelow, ic1fld_ );
    xy0fld_->attach( rightOf, ic0fld_ );
    xy1fld_->attach( rightOf, ic1fld_ );
    xy2fld_->attach( rightOf, ic2fld_ );

    crdgrp_->setHAlignObj( ic0fld_ );
}


void uiSurveyInfoEditor::mkTransfGrp()
{
    trgrp_ = new uiGroup( this, "I/C to X/Y transformation" );
    x0fld_ = new uiGenInput ( trgrp_, tr("X = "), DoubleInpSpec().setName("X"));
    x0fld_->setElemSzPol( uiObject::Small );
    xinlfld_ = new uiGenInput ( trgrp_, tr("+ in-line *"),
				       DoubleInpSpec().setName("Inl") );
    xinlfld_->setElemSzPol( uiObject::Small );
    xcrlfld_ = new uiGenInput ( trgrp_, tr("+ cross-line *"),
				      DoubleInpSpec().setName("Crl") );
    xcrlfld_->setElemSzPol( uiObject::Small );
    y0fld_ = new uiGenInput ( trgrp_, tr("Y = "), DoubleInpSpec().setName("Y"));
    y0fld_->setElemSzPol( uiObject::Small );
    yinlfld_ = new uiGenInput ( trgrp_, tr("+ in-line *"),
				      DoubleInpSpec() .setName("Inl"));
    yinlfld_->setElemSzPol( uiObject::Small );
    ycrlfld_ = new uiGenInput ( trgrp_, tr("+ cross-line *"),
				      DoubleInpSpec() .setName("Crl"));
    ycrlfld_->setElemSzPol( uiObject::Small );
    overrulefld_ = new uiCheckBox( trgrp_, tr("Overrule easy settings") );
    overrulefld_->setChecked( false );
    xinlfld_->attach( rightOf, x0fld_ );
    xcrlfld_->attach( rightOf, xinlfld_ );
    y0fld_->attach( alignedBelow, x0fld_ );
    yinlfld_->attach( rightOf, y0fld_ );
    ycrlfld_->attach( rightOf, yinlfld_ );
    overrulefld_->attach( alignedBelow, ycrlfld_ );
    trgrp_->setHAlignObj( xinlfld_ );
}


static void setZValFld( uiGenInput* zfld, int nr, float val, float fac )
{
    if ( mIsUdf(val) )
	{ zfld->setText( "", nr ); return; }

    val *= fac; int ival = mNINT32(val); float fival = mCast(float,ival);
    if ( mIsEqual(val,fival,0.01) )
	zfld->setValue( ival, nr );
    else
	zfld->setValue( val, nr );
}


void uiSurveyInfoEditor::setValues()
{
    const TrcKeyZSampling& cs = si_.sampling( false );
    const TrcKeySampling& hs = cs.hsamp_;
    StepInterval<int> inlrg( hs.start_.inl(), hs.stop_.inl(), hs.step_.inl() );
    StepInterval<int> crlrg( hs.start_.crl(), hs.stop_.crl(), hs.step_.crl() );
    inlfld_->setValue( inlrg );
    crlfld_->setValue( crlrg );

    const StepInterval<float>& zrg = si_.zRange( false );
    const float zfac = mCast( float, si_.zDomain().userFactor() );
    setZValFld( zfld_, 0, zrg.start, zfac );
    setZValFld( zfld_, 1, zrg.stop, zfac );
    setZValFld( zfld_, 2, zrg.step, zfac );

    x0fld_->setValue( si_.b2c_.getTransform(true).a );
    xinlfld_->setValue( si_.b2c_.getTransform(true).b );
    xcrlfld_->setValue( si_.b2c_.getTransform(true).c );
    y0fld_->setValue( si_.b2c_.getTransform(false).a );
    yinlfld_->setValue( si_.b2c_.getTransform(false).b );
    ycrlfld_->setValue( si_.b2c_.getTransform(false).c );

    Coord c[3]; BinID b[2]; int xline;
    si_.get3Pts( c, b, xline );
    if ( b[0].inl() )
    {
	ic0fld_->setValue( b[0] );
	ic1fld_->setValues( b[0].inl(), xline );
	ic2fld_->setValue( b[1] );
	if ( !c[0].x && !c[0].y && !c[1].x && !c[1].y && !c[2].x && !c[2].y)
	{
	    c[0] = si_.transform( b[0] );
	    c[1] = si_.transform( b[1] );
	    c[2] = si_.transform( BinID(b[0].inl(),xline) );
	}
	xy0fld_->setValue( c[0] );
	xy1fld_->setValue( c[2] );
	xy2fld_->setValue( c[1] );
    }

    const bool zistime = si_.zDomain().isTime();
    const bool xyinfeet = si_.xyInFeet();
    const bool zinfeet = si_.depthsInFeet();
    zunitfld_->setCurrentItem( zistime	? 0 : (zinfeet ? 2 : 1) );
    depthdispfld_->setValue( !zinfeet );
    depthdispfld_->setSensitive( zistime && !xyinfeet );

    const float srd = si_.seismicReferenceDatum();
    const UnitOfMeasure* datauom = zistime || !zinfeet ? UoMR().get( "Meter" )
						       : UoMR().get( "Feet" );
    const UnitOfMeasure* displayuom = depthdispfld_->getBoolValue()
				    ? UoMR().get( "Meter" )
				    : UoMR().get( "Feet" );
    refdatumfld_->setValue( getConvertedValue( srd, datauom, displayuom ) );
}


ObjectSet<uiSurvInfoProvider>& uiSurveyInfoEditor::survInfoProvs()
{
    mDefineStaticLocalObject( PtrMan<ObjectSet<uiSurvInfoProvider> >, sips,
			      = new ObjectSet<uiSurvInfoProvider> );
    return *sips;
}


int uiSurveyInfoEditor::addInfoProvider( uiSurvInfoProvider* p )
{
    if ( p ) survInfoProvs() += p;
    return survInfoProvs().size();
}


bool uiSurveyInfoEditor::copySurv( const char* inpath, const char* indirnm,
				   const char* outpath, const char* outdirnm )
{
    const BufferString fnmin = FilePath(inpath).add(indirnm).fullPath();
    const BufferString fnmout = FilePath(outpath).add(outdirnm).fullPath();
    if ( File::exists(fnmout) )
    {
	uiString msg = tr("Cannot copy %1 to %2"
			  "\nbecause target directory exists")
		     .arg(fnmin).arg(fnmout);
	uiMSG().error( msg );
	return false;
    }
    MouseCursorManager::setOverride( MouseCursor::Wait );
    File::copy( fnmin, fnmout );
    MouseCursorManager::restoreOverride();
    if ( !File::exists(fnmout) )
    {
	uiString msg = tr("Copy %1 to %2 failed\n"
			  "See startup window for details")
		     .arg(fnmin).arg(fnmout);
	uiMSG().error( msg );
	return false;
    }

    return true;
}


bool uiSurveyInfoEditor::renameSurv( const char* path, const char* indirnm,
				     const char* outdirnm )
{
    const BufferString fnmin = FilePath(path).add(indirnm).fullPath();
    const BufferString fnmout = FilePath(path).add(outdirnm).fullPath();
    if ( File::exists(fnmout) )
    {
	uiString msg = tr("Cannot rename %1 to %2"
			  "\nbecause target directory exists")
		     .arg(fnmin).arg(fnmout);
	uiMSG().error( msg );
	return false;
    }
    File::rename( fnmin, fnmout );
    if ( !File::exists(fnmout) )
    {
	uiString msg = tr("Rename %1 to %2 failed\n"
			  "See startup window for details")
		     .arg(fnmin).arg(fnmout);
	uiMSG().error( msg );
	return false;
    }

    return true;
}


#define mUseAdvanced() (overrulefld_->isChecked() && !coordset->getBoolValue())

void uiSurveyInfoEditor::appButPushed( CallBacker* )
{
    doApply();
}


bool uiSurveyInfoEditor::doApply()
{
    if ( !setSurvName() || !setRanges() )
	return false;

    if ( !mUseAdvanced() )
    {
	if ( !setCoords() ) return false;

	x0fld_->setValue( si_.b2c_.getTransform(true).a );
	xinlfld_->setValue( si_.b2c_.getTransform(true).b );
	xcrlfld_->setValue( si_.b2c_.getTransform(true).c );
	y0fld_->setValue( si_.b2c_.getTransform(false).a );
	yinlfld_->setValue( si_.b2c_.getTransform(false).b );
	ycrlfld_->setValue( si_.b2c_.getTransform(false).c );
	overrulefld_->setChecked( false );
    }
    else if ( !setRelation() )
	return false;

    si_.update3DGeometry();
    survParChanged.trigger();
    return true;
}


void uiSurveyInfoEditor::doFinalise( CallBacker* )
{
    pathfld_->setText( orgstorepath_ );
    pathfld_->setReadOnly( true );
    updStatusBar( orgstorepath_ );

    pol2dfld_->setCurrentItem( (int)si_.survDataType() );

    if ( si_.sampling(false).hsamp_.totalNr() )
	setValues();

    chgSetMode(0);
    ic1fld_->setReadOnly( true, 0, 0 );

    sipCB(0);
}


bool uiSurveyInfoEditor::rejectOK( CallBacker* )
{
    if ( isnew_ )
    {
	const BufferString dirnm = FilePath(orgstorepath_).add(orgdirname_)
							  .fullPath();
	if ( File::exists(dirnm) )
	    File::remove( dirnm );
    }

    return true;
}


bool uiSurveyInfoEditor::setSurvName()
{
    BufferString newsurvnm( survnmfld_->text() );
    if ( newsurvnm.size() < 2 )
    {
	uiMSG().error( tr("Please specify a valid survey name") );
	return false;
    }
    si_.setName( newsurvnm );
    return true;
}


bool uiSurveyInfoEditor::acceptOK( CallBacker* )
{
    if ( !doApply() )
	return false;

    const BufferString newstorepath( pathfld_->text() );
    const BufferString newdirnm( dirName() );
    const BufferString olddir(
			FilePath(orgstorepath_).add(orgdirname_).fullPath() );
    const BufferString newdir(FilePath(newstorepath).add(newdirnm).fullPath());
    const bool storepathchanged = orgstorepath_ != newstorepath;
    dirnamechanged = orgdirname_ != newdirnm;

    if ( (dirnamechanged || storepathchanged) && File::exists(newdir) )
    {
	uiMSG().error( tr("The new target directory exists.\n"
		       "Please enter another survey name or location.") );
	return false;
    }

    if ( storepathchanged )
    {
	if ( !uiMSG().askGoOn(tr("Copy your survey to another location?")) )
	    return false;
	else if ( !copySurv(orgstorepath_,orgdirname_,
			    newstorepath,newdirnm) )
	    return false;
	else if ( !uiMSG().askGoOn(tr("Keep the survey at the old location?")) )
	    File::remove( olddir );
    }
    else if ( dirnamechanged )
    {
	if ( !renameSurv(orgstorepath_,orgdirname_,newdirnm) )
	    return false;
    }

    BufferString linkpos = FilePath(rootdir_).add(newdirnm).fullPath();
    if ( File::exists(linkpos) )
    {
       if ( File::isLink(linkpos) )
	   File::remove( linkpos );
    }

    if ( !File::exists(linkpos) )
    {
	if ( !File::createLink(newdir,linkpos) )
	{
	    uiString msg =
		uiStrings::phrCannotCreate( tr("link from \n%1 to \n%2")
					     .arg(newdir).arg(linkpos));
	    uiMSG().error( msg );
	    return false;
	}
    }

    si_.dirname_ = newdirnm;
    si_.setSurvDataType( (SurveyInfo::Pol2D)pol2dfld_->currentItem() );
    if ( mUseAdvanced() )
	si_.get3Pts( si_.set3coords_, si_.set3binids_,
			si_.set3binids_[2].crl() );

    if ( !si_.write(rootdir_) )
    {
       uiMSG().error(tr("Failed to write survey info.\nNo changes committed."));
	return false;
    }

    return true;
}


const char* uiSurveyInfoEditor::dirName() const
{
    mDeclStaticString( ret ); ret = survnmfld_->text();
    ret.clean( BufferString::AllowDots );
    return ret.buf();
}


#define mErrRet(s) { uiMSG().error(s); return false; }


bool uiSurveyInfoEditor::setRanges()
{
    const StepInterval<int> irg( inlfld_->getIStepInterval() );
    const StepInterval<int> crg( crlfld_->getIStepInterval() );
    if ( irg.isUdf() ) mErrRet(tr("Please enter a valid range for inlines"))
    if ( crg.isUdf() ) mErrRet(tr("Please enter a valid range for crosslines"))
    TrcKeyZSampling cs( si_.sampling(false) );
    TrcKeySampling& hs = cs.hsamp_;
    hs.start_.inl() = irg.start; hs.start_.crl() = crg.start;
    hs.stop_.inl() = irg.atIndex( irg.getIndex(irg.stop) );
    hs.stop_.crl() = crg.atIndex( crg.getIndex(crg.stop) );
    hs.step_.inl() = irg.step;	 hs.step_.crl() = crg.step;
    if ( hs.step_.inl() < 1 ) hs.step_.inl() = 1;
    if ( hs.step_.crl() < 1 ) hs.step_.crl() = 1;

    const bool zistime = zunitfld_->currentItem() == 0;
    const bool zinfeet = !depthdispfld_->getBoolValue();
    si_.setZUnit( zistime, zinfeet );
    si_.getPars().setYN( SurveyInfo::sKeyDpthInFt(), zinfeet );

    const float srd = refdatumfld_->getFValue();
    const UnitOfMeasure* datauom = zistime || !zinfeet ? UoMR().get( "Meter" )
						       : UoMR().get( "Feet" );
    const UnitOfMeasure* displayuom = !zinfeet ? UoMR().get( "Meter" )
					       : UoMR().get( "Feet" );
    si_.setSeismicReferenceDatum( getConvertedValue(srd,displayuom,datauom) );

    cs.zsamp_ = zfld_->getFStepInterval();
    if (mIsUdf(cs.zsamp_.start) || mIsUdf(cs.zsamp_.stop)
				|| mIsUdf(cs.zsamp_.step))
	mErrRet(tr("Please enter the Z Range"))
    const float zfac = 1.f / si_.zDomain().userFactor();
    if ( !mIsEqual(zfac,1,0.0001) )
	cs.zsamp_.scale( zfac );

    if ( mIsZero(cs.zsamp_.step,1e-8) )
	cs.zsamp_.step = si_.zDomain().isTime() ? 0.004f : 1;
    cs.normalise();
    if ( !hs.totalNr() )
	mErrRet(tr("Please specify in-line/cross-line ranges"))
    if ( cs.zsamp_.nrSteps() == 0 )
	mErrRet(tr("Please specify a valid Z range"))

    si_.setRange( cs, false );
    si_.setRange( cs, true );
    return true;
}


bool uiSurveyInfoEditor::setCoords()
{
    BinID b[2]; Coord c[3]; int xline;
    b[0] = ic0fld_->getBinID();
    b[1] = ic2fld_->getBinID();
    xline = ic1fld_->getBinID().crl();
    c[0] = xy0fld_->getCoord();
    c[1] = xy2fld_->getCoord();
    c[2] = xy1fld_->getCoord();

    const char* msg = si_.set3Pts( c, b, xline );
    if ( msg ) { uiMSG().error( mToUiStringTodo(msg) ); return false; }
    else if ( mUseAdvanced() )
	si_.gen3Pts();

    if ( coordsystem_ )
	si_.setCoordSystem( coordsystem_ );

    return true;
}


bool uiSurveyInfoEditor::setRelation()
{
    Pos::IdxPair2Coord::DirTransform xtr, ytr;
    xtr.a = x0fld_->getdValue();   ytr.a = y0fld_->getdValue();
    xtr.b = xinlfld_->getdValue(); ytr.b = yinlfld_->getdValue();
    xtr.c = xcrlfld_->getdValue(); ytr.c = ycrlfld_->getdValue();
    if ( !xtr.valid(ytr) )
    {
	uiMSG().error( tr("The transformation is not valid.") );
	return false;
    }

    si_.b2c_.setTransforms( xtr, ytr );
    return true;
}


void uiSurveyInfoEditor::updatePar( CallBacker* cb )
{}


void uiSurveyInfoEditor::sipCB( CallBacker* cb )
{
    const int sipidx = sipfld_ ? sipfld_->currentItem() : 0;
    if ( sipidx < 1 ) return;
    sipfld_->setCurrentItem( 0 );
    delete impiop_; impiop_ = 0; lastsip_ = 0;

    uiSurvInfoProvider* sip = sips_[sipidx-1];
    PtrMan<uiDialog> dlg = sip->dialog( this );
    if ( !dlg || !dlg->go() ) return;

    TrcKeyZSampling cs; Coord crd[3];
    if ( !sip->getInfo(dlg,cs,crd) )
	return;

    IOPar& pars = si_.getPars();
    sip->fillPar( pars );
    PtrMan<IOPar> crspar = sip->getCoordSystemPars();
    RefMan<Coords::CoordSystem> coordsys =
		crspar ? Coords::CoordSystem::createSystem( *crspar ) : 0;

    if ( !coordsys )
    {
	Coord llcrd; LatLong llll;
	if ( sip->getLatLongAnchor(llcrd,llll) )
	{
	    RefMan<Coords::AnchorBasedXY> anchoredsystem =
				new Coords::AnchorBasedXY( llll, llcrd );
	    anchoredsystem->setIsFeet( sip->xyInFeet() );
	    coordsys = anchoredsystem;
	}
    }

    if ( coordsys )
    {
	si_.setCoordSystem( coordsys );
	coordsystem_ = coordsys;
    }

    const bool xyinfeet = si_.getCoordSystem()->isFeet();
    uiSurvInfoProvider::TDInfo tdinfo = sip->tdInfo();
    bool zistime = si_.zIsTime();
    if ( tdinfo != uiSurvInfoProvider::Uknown )
	zistime = tdinfo == uiSurvInfoProvider::Time;
    bool zinfeet = !depthdispfld_->getBoolValue();
    if ( zistime )
    {
	if ( xyinfeet )
	    zinfeet = true;
    }
    else
	zinfeet = sip->tdInfo() == uiSurvInfoProvider::DepthFeet;

    si_.setZUnit( zistime, zinfeet );
    pars.setYN( SurveyInfo::sKeyDpthInFt(), zinfeet );
    si_.setXYInFeet( xyinfeet );
    xyunitlbl_->setText( getCoordString(xyInFeet()) );

    float srd = 0.f;
    if ( sip->getSRD(srd) && !mIsUdf(srd) )
    {
	if ( !zistime && zinfeet )
	    srd *= mToFeetFactorF;
	si_.setSeismicReferenceDatum( srd );
    }

    const bool havez = !mIsUdf(cs.zsamp_.start);
    if ( !havez )
	cs.zsamp_ = si_.zRange(false);

    si_.setRange(cs,false);
    BinID bid[2];
    bid[0].inl() = cs.hsamp_.start_.inl();
    bid[0].crl() = cs.hsamp_.start_.crl();
    bid[1].inl() = cs.hsamp_.stop_.inl();
    bid[1].crl() = cs.hsamp_.stop_.crl();
    si_.set3Pts( crd, bid, cs.hsamp_.stop_.crl() );
    setValues();
    if ( !havez )
	zfld_->clear();

    lastsip_ = sip;
    impiop_ = lastsip_->getImportPars();
}


void uiSurveyInfoEditor::pathbutPush( CallBacker* )
{
    uiFileDialog dlg( this, uiFileDialog::DirectoryOnly, pathfld_->text() );
    if ( dlg.go() )
    {
	BufferString dirnm( dlg.fileName() );
	if ( !File::isWritable(dirnm) )
	{
	    uiMSG().error( tr("Directory is not writable") );
	    return;
	}
	updStatusBar( dirnm );
	pathfld_->setText( dirnm );
    }
}


void uiSurveyInfoEditor::updStatusBar( const char* dirnm )
{
    BufferString msg;
    System::getFreeMBOnDiskMsg( System::getFreeMBOnDisk(dirnm), msg );
    toStatusBar( mToUiStringTodo(msg) );
}


void uiSurveyInfoEditor::chgSetMode( CallBacker* )
{
    crdgrp_->display( coordset->getBoolValue() );
    trgrp_->display( !coordset->getBoolValue() );
}


void uiSurveyInfoEditor::setInl1Fld( CallBacker* )
{
    ic1fld_->setText( ic0fld_->text(0), 0 );
}


bool uiSurveyInfoEditor::xyInFeet() const
{
    return coordsystem_ ? coordsystem_->isFeet() : false;
}


void uiSurveyInfoEditor::coordSystemCB( CallBacker* )
{
    Coords::uiCoordSystemDlg dlg( this, true, false, &si_, coordsystem_ );
    if ( !dlg.go() )
	return;

    coordsystem_ = dlg.getCoordSystem();
    updZUnit(0);
    xyunitlbl_->setText( getCoordString(xyInFeet()) );
}


void uiSurveyInfoEditor::rangeChg( CallBacker* cb )
{
    if ( cb == inlfld_ )
    {
	StepInterval<int> irg = inlfld_->getIStepInterval();
	if ( mIsUdf(irg.step) || !irg.step || irg.step>irg.width() )irg.step=1;
	if ( irg.isUdf() ) return;

	irg.stop = irg.atIndex( irg.getIndex(irg.stop) );
	inlfld_->setValue( irg );
    }
    else if ( cb == crlfld_ )
    {
	StepInterval<int> crg = crlfld_->getIStepInterval();
	if ( mIsUdf(crg.step) || !crg.step || crg.step>crg.width() )crg.step=1;
	if ( crg.isUdf() ) return;

	crg.stop = crg.atIndex( crg.getIndex(crg.stop) );
	crlfld_->setValue( crg );
    }
    else if ( cb == zfld_ )
    {
	StepInterval<double> zrg = zfld_->getDStepInterval();
	if ( mIsUdf(zrg.step) || mIsZero(zrg.step,1e-6) ) return;

	zrg.stop = zrg.atIndex( zrg.getIndex(zrg.stop) );
	zfld_->setValue( zrg );
    }
}


void uiSurveyInfoEditor::depthDisplayUnitSel( CallBacker* )
{
    const bool showdepthinft = !depthdispfld_->getBoolValue();
    refdatumfld_->setTitleText( getSRDString(showdepthinft) );
    float refdatum = refdatumfld_->getfValue();
    refdatum *= showdepthinft ? mToFeetFactorF : mFromFeetFactorF;
    refdatumfld_->setValue( refdatum );
}


void uiSurveyInfoEditor::updZUnit( CallBacker* )
{
    const UnitOfMeasure* prevdisplayuom = depthdispfld_->getBoolValue()
					? UoMR().get( "Meter" )
					: UoMR().get( "Feet" );
    const float oldsrduser = refdatumfld_->getFValue();

    const bool zintime = zunitfld_->currentItem() == 0;
    const bool zinft = zunitfld_->currentItem() == 2;
    const bool xyinft = xyInFeet();
    depthdispfld_->setSensitive( zintime && !xyinft );
    if ( zintime )
    {
	if ( xyinft )
	    depthdispfld_->setValue( false );
    }
    else
	depthdispfld_->setValue( !zinft );

    const bool showdepthinft = !depthdispfld_->getBoolValue();
    const UnitOfMeasure* newdisplayuom = !showdepthinft ? UoMR().get( "Meter" )
							: UoMR().get( "Feet" );
    const float newsrduser = getConvertedValue( oldsrduser, prevdisplayuom,
						newdisplayuom );
    refdatumfld_->setValue( newsrduser );
    refdatumfld_->setTitleText( getSRDString(showdepthinft) );
    const UnitOfMeasure* datauom = zintime || !zinft ? UoMR().get( "Meter" )
						     : UoMR().get( "Feet" );
    si_.setSeismicReferenceDatum( getConvertedValue(newsrduser,newdisplayuom,
						    datauom) );
}



// uiCopySurveySIP

static HiddenParam<uiCopySurveySIP,IOPar*> crspars_(0);

uiCopySurveySIP::uiCopySurveySIP()
{
    crspars_.setParam( this, 0 );
}


void uiCopySurveySIP::reset()
{
    if ( crspars_.hasParam(this) )
	crspars_.removeAndDeleteParam( this );
}


uiDialog* uiCopySurveySIP::dialog( uiParent* p )
{
    survlist_.erase();
    uiSurveySelectDlg* dlg = new uiSurveySelectDlg( p, GetSurveyName(),
	GetBaseDataDir(), true, true );
    dlg->setHelpKey( mODHelpKey(mCopySurveySIPHelpID) );
    return dlg;
}


bool uiCopySurveySIP::getInfo(uiDialog* dlg, TrcKeyZSampling& cs, Coord crd[3])
{
    tdinf_ = Uknown;
    inft_ = false;
    mDynamicCastGet(uiSurveySelectDlg*,seldlg,dlg)
    if ( !seldlg ) return false;

    const BufferString fname = seldlg->getSurveyPath();
    PtrMan<SurveyInfo> survinfo = SurveyInfo::read( fname );
    if ( !survinfo ) return false;

    cs = survinfo->sampling( false );
    crd[0] = survinfo->transform( cs.hsamp_.start_ );
    crd[1] = survinfo->transform( cs.hsamp_.stop_ );
    crd[2] = survinfo->transform(
	BinID(cs.hsamp_.start_.inl(),cs.hsamp_.stop_.crl()));

    tdinf_ = survinfo->zIsTime() ? Time
				 : (survinfo->zInFeet() ? DepthFeet : Depth);
    inft_ = survinfo->xyInFeet();

    RefMan<Coords::CoordSystem> crs = survinfo->getCoordSystem();
    IOPar* crspar = new IOPar;
    crs->fillPar( *crspar );
    if ( crspars_.hasParam(this) )
	delete crspars_.getParam( this );

    crspars_.setParam( this, crspar );

    return true;
}


IOPar* uiCopySurveySIP::getCoordSystemPars() const
{
    if ( !crspars_.hasParam(this) )
       return 0;

    IOPar* par = new IOPar( *crspars_.getParam( this ) );
    return par;
}


// uiSurveyFileSIP
uiSurveyFileSIP::uiSurveyFileSIP()
{}


const char* uiSurveyFileSIP::usrText() const
{
    return "Read from Survey Setup file";
}


class uiSurveyFileDlg : public uiDialog
{ mODTextTranslationClass(uiSurveyFileDlg)
public:
uiSurveyFileDlg( uiParent* p )
    : uiDialog(p,Setup(tr("Select Survey Setup file"),
			mNoDlgTitle,mTODOHelpKey))
{
    inpfld_ = new uiFileInput( this, uiStrings::sSelect(),
		uiFileInput::Setup().defseldir(GetBaseDataDir())
				    .withexamine(true)
				    .allowallextensions(true) );
    inpfld_->setElemSzPol( uiObject::Wide );
}

uiFileInput* inpfld_;

};


uiDialog* uiSurveyFileSIP::dialog( uiParent* p )
{
    return new uiSurveyFileDlg( p );
}


bool uiSurveyFileSIP::getInfo( uiDialog* dlg, TrcKeyZSampling& cs, Coord crd[3])
{
    tdinf_ = Uknown;
    inft_ = false;
    mDynamicCastGet(uiSurveyFileDlg*,filedlg,dlg)
    if ( !filedlg ) return false;

    const BufferString fname = filedlg->inpfld_->fileName();
    PtrMan<SurveyInfo> survinfo = SurveyInfo::read( fname, true );
    if ( !survinfo ) return false;

    cs = survinfo->sampling( false );
    crd[0] = survinfo->transform( cs.hsamp_.start_ );
    crd[1] = survinfo->transform( cs.hsamp_.stop_ );
    crd[2] = survinfo->transform(
	BinID(cs.hsamp_.start_.inl(),cs.hsamp_.stop_.crl()));

    tdinf_ = survinfo->zIsTime() ? Time
				 : (survinfo->zInFeet() ? DepthFeet : Depth);
    inft_ = survinfo->xyInFeet();
    coordsystem_ = survinfo->getCoordSystem();
    surveynm_ = survinfo->name();

    return true;
}


IOPar* uiSurveyFileSIP::getCoordSystemPars() const
{
    if ( !coordsystem_ )
	return 0;

    IOPar* crspar = new IOPar;
    coordsystem_->fillPar( *crspar );
    return crspar;
}
