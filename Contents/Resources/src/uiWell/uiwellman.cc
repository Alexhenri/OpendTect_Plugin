/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        Nanne Hemstra
 Date:          September 2003
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "uiwellman.h"

#include "bufstringset.h"
#include "ioobj.h"
#include "ioman.h"
#include "ctxtioobj.h"
#include "file.h"
#include "filepath.h"
#include "ptrman.h"
#include "strmprov.h"
#include "survinfo.h"
#include "unitofmeasure.h"
#include "welldata.h"
#include "welltrack.h"
#include "welllog.h"
#include "welllogset.h"
#include "welld2tmodel.h"
#include "wellman.h"
#include "wellmarker.h"
#include "wellreader.h"
#include "welltransl.h"
#include "wellwriter.h"

#include "uitoolbutton.h"
#include "uigeninputdlg.h"
#include "uigroup.h"
#include "uiioobjmanip.h"
#include "uiioobjselgrp.h"
#include "uilistbox.h"
#include "uimsg.h"
#include "uistrings.h"
#include "uitextedit.h"
#include "uitoolbar.h"
#include "uiwelldlgs.h"
#include "uiwelllogimpexp.h"
#include "uiwelllogcalc.h"
#include "uiwelllogtools.h"
#include "uiwellmarkerdlg.h"
#include "uiwelllogdisplay.h"
#include "od_helpids.h"

mDefineInstanceCreatedNotifierAccess(uiWellMan)


uiWellMan::uiWellMan( uiParent* p )
    : uiObjFileMan(p,uiDialog::Setup(
		uiStrings::phrManage( uiStrings::sWell(mPlural)),mNoDlgTitle,
				      mODHelpKey(mWellManHelpID))
		     .nrstatusflds(1).modal(false),
		   WellTranslatorGroup::ioContext() )
    , d2tbut_(0)
    , csbut_(0)
    , iswritable_(true)
{
    createDefaultUI( false, true, true );
    setPrefWidth( 50 );

    if ( SI().zIsTime() )
    {
	uiIOObjManipGroup* manipgrp = selgrp_->getManipGroup();
	manipgrp->addButton( "z2t",
			tr("Set Depth to Time Model from other Well"),
			mCB(this,uiWellMan,bulkD2TCB) );
    }

    logsgrp_ = new uiGroup( listgrp_, "Logs group" );
    uiListBox::Setup su( OD::ChooseAtLeastOne, uiStrings::sLogs(),
			 uiListBox::AboveMid );
    logsfld_ = new uiListBox( logsgrp_, su );
    logsfld_->setHSzPol( uiObject::Wide );

    uiButtonGroup* logsbgrp = new uiButtonGroup( listgrp_, "Logs buttons",
						 OD::Horizontal );
    addlogsbut_ = new uiPushButton( logsbgrp, uiStrings::sImport(), false );
    addlogsbut_->activated.notify( mCB(this,uiWellMan,importLogs) );
    calclogsbut_ = new uiPushButton( logsbgrp, uiStrings::sCreate(), false);
    calclogsbut_->activated.notify( mCB(this,uiWellMan,calcLogs) );
    calclogsbut_->attach( rightOf, addlogsbut_ );
    logsbgrp->attach( centeredBelow, logsgrp_ );

    uiManipButGrp* butgrp = new uiManipButGrp( logsfld_ );
    logvwbut_ = butgrp->addButton( "view_log", mJoinUiStrs(sView(),
					      sSelectedLog().toLower()),
					      mCB(this,uiWellMan,viewLogPush) );
    logrenamebut_ = butgrp->addButton( uiManipButGrp::Rename,
		      uiStrings::phrRename(uiStrings::sSelectedLog().toLower()),
		      mCB(this,uiWellMan,renameLogPush) );
    logrmbut_ = butgrp->addButton( uiManipButGrp::Remove,
		uiStrings::phrRemove(uiStrings::sSelectedLog(mPlural).toLower())
		, mCB(this,uiWellMan,removeLogPush) );
    logexpbut_ = butgrp->addButton( "export",
			uiStrings::phrExport( uiStrings::sWellLog(mPlural) ),
			mCB(this,uiWellMan,exportLogs) );
    loguombut_ = butgrp->addButton( "unitsofmeasure",
					tr("View/edit unit of measure"),
					mCB(this,uiWellMan,logUOMPush) );
    logedbut_ = butgrp->addButton( "edit", uiStrings::sEdit(),
			mCB(this,uiWellMan,editLogPush) );
    logupbut_ = butgrp->addButton( "uparrow", uiStrings::sMoveUp(),
			mCB(this,uiWellMan,moveLogsPush) );
    logdownbut_ = butgrp->addButton( "downarrow", uiStrings::sMoveDown(),
			mCB(this,uiWellMan,moveLogsPush) );
    logsfld_->selectionChanged.notify( mCB(this,uiWellMan,logSel) );
    logsfld_->itemChosen.notify( mCB(this,uiWellMan,logSel) );
    butgrp->attach( rightOf, logsfld_->box() );
    logsgrp_->attach( rightOf, selgrp_ );

    welltrackbut_ = new uiToolButton( extrabutgrp_, "edwelltrack",
			uiStrings::phrEdit(mJoinUiStrs(sWell(),sTrack())),
			mCB(this,uiWellMan, edWellTrack) );

    if ( SI().zIsTime() )
    {
	csbut_ = new uiToolButton( extrabutgrp_, "checkshot",
			    uiStrings::phrEdit(tr("Checkshot Data")),
			    mCB(this,uiWellMan,edChckSh) );
	d2tbut_ = new uiToolButton( extrabutgrp_,
			    "z2t",uiStrings::phrEdit(tr("Depth/Time Model")),
			    mCB(this,uiWellMan, edD2T) );
    }

    markerbut_ = new uiToolButton( extrabutgrp_, "edmarkers",
			      uiStrings::phrEdit( uiStrings::sMarker(mPlural) ),
			      mCB(this,uiWellMan, edMarkers) );
    markerbut_->attach( rightOf, d2tbut_ ? d2tbut_ : welltrackbut_ );

    new uiToolButton( extrabutgrp_, "tools",
		      tr("Log tools"), mCB(this,uiWellMan,logTools) );
    extrabutgrp_->attach( ensureBelow, logsgrp_ );

    selChg( this );
    mTriggerInstanceCreatedNotifier();
}


uiWellMan::~uiWellMan()
{
    deepErase( currdrs_ );
    deepUnRef( curwds_ );
}


void uiWellMan::ownSelChg()
{
    iswritable_ = curioobj_ && Well::Writer::isFunctional(*curioobj_);
    getCurrentWells();
    fillLogsFld();
    setWellToolButtonProperties();
    setLogToolButtonProperties();
}


static void getBasicInfo( Well::Reader* rdr )
{
    if ( rdr )
    {
	rdr->getTrack();
	rdr->getInfo();
    }
}


void uiWellMan::getCurrentWells()
{
    curfnms_.erase();
    deepErase( currdrs_ );
    deepUnRef( curwds_ );
    curmultiids_.erase();

    if ( !curioobj_ )
	return;

    const int nrsel = selGroup()->nrChosen();
    for ( int idx=0; idx<nrsel; idx++ )
    {
	const IOObj* obj = IOM().get( selgrp_->chosenID(idx) );
	if ( !obj ) continue;

	curmultiids_ += obj->key();
	curfnms_.add( BufferString( obj->fullUserExpr( true ) ) );
	Well::Data* wd = new Well::Data;
	curwds_ += wd;
	currdrs_ += new Well::Reader( *obj, *curwds_[idx] );
	getBasicInfo( currdrs_[idx] );
    }

    deepRef( curwds_ );
}


void uiWellMan::bulkD2TCB( CallBacker* )
{
    uiSetD2TFromOtherWell dlg( this );
    dlg.setSelected( getSelWells() );
    if ( !dlg.go() )
	return;

    // update display?
}


void uiWellMan::fillLogsFld()
{
    logsfld_->setEmpty();
    if ( currdrs_.isEmpty() )
	return;

    availablelognms_.erase();
    currdrs_[0]->getLogInfo( availablelognms_ );
    if ( currdrs_.size() > 1 )
    {
	for ( int idx=1; idx<currdrs_.size(); idx++ )
	{
	    BufferStringSet lognms;
	    currdrs_[idx]->getLogInfo( lognms );
	    for ( int idy=0; idy<availablelognms_.size(); )
	    {
		if ( !lognms.isPresent(availablelognms_.get(idy)) )
		    availablelognms_.removeSingle( idy );
		else
		    idy++;
	    }
	}
    }

    for ( int idx=0; idx<availablelognms_.size(); idx++)
	logsfld_->addItem( toUiString(availablelognms_.get(idx)) );

    logsfld_->chooseAll( false );
    addlogsbut_->setSensitive( iswritable_ && curwds_.size() == 1 );
    calclogsbut_->setSensitive( iswritable_ );

    logSel(0);
}



void uiWellMan::setButToolTip( uiButton* but, const uiString& oper,
			   const uiString& objtyp, const uiString& obj,
			   const uiString& end )
{
    if ( !but )
	return;

    uiString tt = toUiString("%1 %2").arg(oper)
				     .arg(objtyp);
    if ( but->sensitive() && !obj.isEmpty() )
	tt = tr("%1 for '%2'").arg(tt).arg(obj);

    if ( !end.isEmpty() )
	tt = toUiString("%1 %2").arg(tt).arg(end);

    but->setToolTip( tt );
}


#define mSetWellButToolTip(but,objtyp) \
    setButToolTip( but, edvwstr, objtyp, curwellnm )


void uiWellMan::setWellToolButtonProperties()
{
    const uiString curwellnm = curioobj_ ? curioobj_->uiName() :
						      uiStrings::sEmptyString();
    const uiString edvwstr = iswritable_ ? uiStrings::sEdit() :
							     uiStrings::sView();

    mSetWellButToolTip( welltrackbut_, mJoinUiStrs(sWell(),sTrack()) );
    if ( d2tbut_ )
	mSetWellButToolTip( d2tbut_, tr("Depth/Time model") );

    if ( csbut_ )
	mSetWellButToolTip( csbut_, tr("Checkshot Data") );

    mSetWellButToolTip( markerbut_, uiStrings::sMarker(mPlural) );
}


#define mSetLogButToolTip(but,oper,end) \
    setButToolTip( but, oper, curlognm, curwellnm, end )


void uiWellMan::setLogToolButtonProperties()
{
    const int nrlogs = logsfld_->size();
    const int curidx = logsfld_->currentItem();
    logdownbut_->setSensitive( iswritable_ && curidx>=0 && curidx<nrlogs-1 );
    logupbut_->setSensitive( iswritable_ && curidx>0 );

    TypeSet<MultiID> wellids; selGroup()->getChosen( wellids );
    BufferStringSet lognms; logsfld_->getChosen( lognms );
    const int nrchosenlogs = lognms.size();
    const int nrchosenwells = wellids.size();
    const bool oneormorelog = nrchosenlogs > 0;

    logrenamebut_->setSensitive( iswritable_ && nrlogs > 0 );
    logrmbut_->setSensitive( iswritable_ && oneormorelog );
    logexpbut_->setSensitive( oneormorelog );
    loguombut_->setSensitive( iswritable_ && nrlogs > 0 );
    logedbut_->setSensitive( iswritable_ && nrlogs > 0 );

    const uiString curwellnm = curioobj_ ? curioobj_->uiName() :
						      uiStrings::sEmptyString();
    const uiString curlognm = toUiString(logsfld_->getText());

    mSetLogButToolTip( logupbut_, uiStrings::sMove(),
						   uiStrings::sUp().toLower() );
    mSetLogButToolTip( logdownbut_, uiStrings::sMove(),
						 uiStrings::sDown().toLower() );
    mSetLogButToolTip( logrenamebut_, uiStrings::sRename(),
						    uiStrings::sEmptyString() );
    mSetLogButToolTip( loguombut_, tr("View/edit units of measure for "),
						    uiStrings::sEmptyString() );
    mSetLogButToolTip( logedbut_, uiStrings::sEdit(),
						    uiStrings::sEmptyString() );

    setButToolTip(logrmbut_, uiStrings::sRemove(),
		  toUiString(lognms.getDispString(3)), curwellnm,
		  uiStrings::sEmptyString());
    setButToolTip(logexpbut_, uiStrings::sExport(),
		  toUiString(lognms.getDispString(3)),
		  nrchosenwells==1 ? curwellnm : uiStrings::sEmptyString(),
		  uiStrings::sEmptyString() );

    const int nrlogs2vw = nrchosenwells * nrchosenlogs ;
    const bool canview = nrlogs2vw == 1 || nrlogs2vw == 2;
    logvwbut_->setSensitive( canview );

    if ( !canview )
	logvwbut_->setToolTip( mJoinUiStrs(sView(),sLog(mPlural).toLower()) );
    else
    {
	BufferStringSet wellnms;
	for ( int midx=0; midx<wellids.size(); midx++ )
	{
	    IOObj* ioobj = IOM().get( wellids[midx] );
	    if ( ioobj )
		wellnms.add( ioobj->name() );
	    delete ioobj;
	}

	uiString tt = tr("View %1 for %2")
		      .arg(toUiString(lognms.getDispString(2)))
		      .arg(toUiString(wellnms.getDispString(2)));
	logvwbut_->setToolTip( tt );
    }
}


void uiWellMan::logSel( CallBacker* )
{
    setLogToolButtonProperties();
}


#define mErrRet(msg) \
{ uiMSG().error(msg); return; }


void uiWellMan::edMarkers( CallBacker* )
{
    if ( !curioobj_ )
	return;

    const MultiID curmid( curioobj_->key() );
    RefMan<Well::Data> wd = new Well::Data;
    if ( Well::MGR().isLoaded(curmid) )
	wd = Well::MGR().get( curmid );
    else
    {
	PtrMan<Well::Reader> wrdr = new Well::Reader( *curioobj_, *wd );
	if ( !wrdr->getMarkers() &&
	     !uiMSG().askGoOn(tr("No markers found. Continue editing?")) )
	    return;
    }

    if ( !iswritable_ )
    {
	uiMarkerViewDlg dlg( this, *wd );
	dlg.go(); return;
    }

    const Well::MarkerSet origmarkers = wd->markers();

    wd->track().setName( curioobj_->name() );
    uiMarkerDlg dlg( this, wd->track() );
    dlg.setMarkerSet( wd->markers() );
    if ( !dlg.go() )
	return;

    dlg.getMarkerSet( wd->markers() );
    Well::Writer wtr( curmid, *wd );
    if ( !wtr.putMarkers() )
    {
	uiMSG().error( tr("Cannot write new markers to disk") );
	wd->markers() = origmarkers;
    }

    wd->markerschanged.trigger();
}


void uiWellMan::edWellTrack( CallBacker* )
{
    if ( !curioobj_ )
	return;

    const MultiID curmid( curioobj_->key() );
    RefMan<Well::Data> wd = new Well::Data;
    if ( Well::MGR().isLoaded(curmid) )
	wd = Well::MGR().get( curmid );
    else
    {
	PtrMan<Well::Reader> wrdr = new Well::Reader( *curioobj_, *wd );
	const bool found = wrdr->getInfo() && wrdr->getTrack();
	if ( !found &&
	     !uiMSG().askGoOn(tr("No track found. Continue editing?")) )
	    return;
    }

    const Well::Track origtrck = wd->track();
    const Coord origpos = wd->info().surfacecoord;
    const float origgl = wd->info().groundelev;

    uiWellTrackDlg dlg( this, *wd );
    if ( !dlg.go() || !iswritable_ )
	return;

    Well::Writer wtr( curmid, *wd );
    if ( !wtr.putInfoAndTrack( ) )
    {
	uiMSG().error( tr("Cannot write new track to disk") );
	wd->track() = origtrck;
	wd->info().surfacecoord = origpos;
	wd->info().groundelev = origgl;
    }

    wd->trackchanged.trigger();
    mkFileInfo();
}


void uiWellMan::edD2T( CallBacker* )
{
    defD2T( false );
}


void uiWellMan::edChckSh( CallBacker* )
{
    defD2T( true );
}


void uiWellMan::defD2T( bool chkshot )
{
    if ( curwds_.isEmpty() || currdrs_.isEmpty() ) return;

    const MultiID curmid = curioobj_->key();
    RefMan<Well::Data> wd = new Well::Data;
    if ( Well::MGR().isLoaded(curmid) )
	wd = Well::MGR().get( curmid );
    else
    {
	PtrMan<Well::Reader> wrdr = new Well::Reader( *curioobj_, *wd );
	if ( chkshot )
	    wrdr->getCSMdl();
	else
	    wrdr->getD2T();
    }

    if ( !chkshot && !wd->d2TModel() )
	wd->setD2TModel( new Well::D2TModel );
    if ( chkshot && !wd->checkShotModel() )
	wd->setCheckShotModel( new Well::D2TModel );

    const float oldreplvel = wd->info().replvel;
    Well::D2TModel* inpmdl = chkshot ? wd->checkShotModel() : wd->d2TModel();
    PtrMan<Well::D2TModel> origd2t = 0;
    if ( inpmdl )
	origd2t = new Well::D2TModel( *inpmdl );

    uiD2TModelDlg dlg( this, *wd, chkshot );
    if ( !dlg.go() || !iswritable_ )
	return;

    uiString errmsg;
    Well::Writer wtr( curmid, *wd );
    if ( (!chkshot && !wtr.putD2T()) || (chkshot && !wtr.putCSMdl()) )
    {
	errmsg = tr("Cannot write new model to disk");
	Well::D2TModel* toput = origd2t.release();
	if ( chkshot )
	    wd->setCheckShotModel( toput );
	else
	{
	    wd->setD2TModel( toput );
	    wd->info().replvel = oldreplvel;
	}
    }
    else if ( !mIsEqual(oldreplvel,wd->info().replvel,1e-2f) &&
	      !wtr.putInfoAndTrack() )
    {
	if ( !errmsg.isEmpty() )
	errmsg.append( tr("Cannot write new %1 to disk")
		       .arg(Well::Info::sReplVel()),true );
	wd->info().replvel = oldreplvel;
    }

    if ( !errmsg.isEmpty() )
	uiMSG().error( errmsg );

    wd->d2tchanged.trigger();
    wd->trackchanged.trigger();
    mkFileInfo();
}


void uiWellMan::logTools( CallBacker* )
{
    BufferStringSet wellnms, lognms;
    logsfld_->getChosen( lognms );
    TypeSet<MultiID> chosnmids;
    selGroup()->getChosen( chosnmids );
    for ( int midx=0; midx<chosnmids.size(); midx ++ )
    {
	const IOObj* ioobj = IOM().get( chosnmids[midx] );
	if ( ioobj )
	    wellnms.add( ioobj->name() );
    }

    uiWellLogToolWinMgr tooldlg( this, &wellnms, &lognms );
    tooldlg.go();
}


#define mDeleteLogs(idx) \
    if ( !curwds_.validIdx( idx ) ) return;\
    while ( curwds_[idx]->logs().size() ) \
	delete curwds_[idx]->logs().remove(0);

void uiWellMan::importLogs( CallBacker* )
{
    uiImportLogsDlg dlg( this, curioobj_, true );
    if ( dlg.go() )
	wellsChgd();
}


void uiWellMan::calcLogs( CallBacker* )
{
    if ( curwds_.isEmpty() || currdrs_.isEmpty()
	|| availablelognms_.isEmpty() || curmultiids_.isEmpty() ) return;

    currdrs_[0]->getLogs();
    uiWellLogCalc dlg( this, curmultiids_ );
    dlg.go();
    if ( dlg.haveNewLogs() )
	wellsChgd();
}


void uiWellMan::logUOMPush( CallBacker* )
{
    if ( curwds_.isEmpty() || currdrs_.isEmpty() ) return;
    BufferStringSet lognms;
    logsfld_->getChosen( lognms );
    if ( lognms.isEmpty() )
	mErrRet(uiStrings::sNoLogSel())

    BufferStringSet wellnms;
    selGroup()->getListField()->getChosen( wellnms );
    const int nrchosenwls = selGroup()->getListField()->nrChosen();
    ObjectSet<Well::LogSet> wls;
    for  ( int widx=0; widx<nrchosenwls; widx++ )
    {
	currdrs_[widx]->getLogs();
	wls += &curwds_[widx]->logs();
    }

    uiWellLogUOMDlg dlg( this, wls, wellnms, lognms );
    if ( !dlg.go() )
	return;

    writeLogs();
}


void uiWellMan::editLogPush( CallBacker* )
{
    if ( curwds_.isEmpty() || currdrs_.isEmpty() ) return;
    const int selidx = logsfld_->firstChosen();
    if ( selidx < 0 )
	mErrRet(uiStrings::sNoLogSel())

    currdrs_[0]->getLogs();
    const char* lognm = logsfld_->textOfItem( selidx );
    Well::LogSet& wls = curwds_[0]->logs();
    const int curlogidx = wls.indexOf( lognm );
    if ( curlogidx < 0 )
	mErrRet(uiStrings::phrCannotRead(uiStrings::sWellLog()))

    Well::Log& wl = wls.getLog( curlogidx );
    uiWellLogEditor dlg( this, wl );
    if ( !dlg.go() || !dlg.isLogChanged() )
	return;

    const bool res = uiMSG().askSave(
			tr("One or more log values have been changed."
			   "\n\nDo you want to save your changes?"), false );
    if ( !res ) return;

    wl.updateAfterValueChanges();
    writeLogs();
}


void uiWellMan::moveLogsPush( CallBacker* cb )
{
    if ( curwds_.isEmpty() || currdrs_.isEmpty() ) return;

    mDynamicCastGet(uiToolButton*,toolbut,cb);
    if ( toolbut != logupbut_ && toolbut != logdownbut_ )
	return;
    bool isup = toolbut == logupbut_;
    const int curlogidx = logsfld_->currentItem();
    const int newlogidx = curlogidx + ( isup ? -1 : 1 );
    Well::LogSet& wls = curwds_[0]->logs();
    currdrs_[0]->getLogs();
    if ( !wls.validIdx( curlogidx ) || !wls.validIdx( newlogidx ) )
	return;
    wls.swap( curlogidx, newlogidx );

    writeLogs();
    logsfld_->setCurrentItem( newlogidx );
}


void uiWellMan::writeLogs()
{
    for ( int idwell=0; idwell<curwds_.size(); idwell++ )
    {
	Well::Writer wwr( curmultiids_[idwell], *curwds_[idwell] );
	if ( !wwr.putLogs() )
	    uiMSG().error( mToUiStringTodo(wwr.errMsg()) );
    }
    wellsChgd();
}


void uiWellMan::wellsChgd()
{
    for ( int idwell=0; idwell<curwds_.size(); idwell++ )
    {
	fillLogsFld();
	Well::MGR().reload( curmultiids_[idwell] );
	mDeleteLogs(idwell);
    }
}


#define mEnsureLogSelected(msgtxt) \
    if ( logsfld_->isEmpty() ) return; \
    const int nrsel = logsfld_->nrChosen(); \
    if ( nrsel < 1 ) \
	mErrRet(msgtxt)




void uiWellMan::viewLogPush( CallBacker* )
{
    mEnsureLogSelected(uiStrings::sNoLogSel())
    currdrs_[0]->getLogs();
    const Well::LogSet& wls = curwds_[0]->logs();
    const char* lognm = logsfld_->textOfItem( logsfld_->firstChosen() );
    const Well::Log* wl = wls.getLog( lognm );
    if ( !wl )
	mErrRet( uiStrings::phrCannotRead( uiStrings::sWellLog() ) )

    BufferStringSet lognms;
    logsfld_->getChosen( lognms );
    const int maxnrchosen = curwds_.size()*lognms.size();

    if ( !maxnrchosen || maxnrchosen > 2 )
	return;

    const Well::Log* wl1=0;
    const Well::Log* wl2=0;
    if ( curwds_[0] )
    {
	const Well::LogSet& wls1 = curwds_[0]->logs();
	wl1 = wls1.getLog( lognms.get(0) );
	if (  lognms.size() == 2 )
	    wl2 = wls1.getLog( lognms.get(1) );
    }

    if ( curwds_.size() > 1 && curwds_[1] )
    {
	currdrs_[1]->getLogs();
	const Well::LogSet& wls2 = curwds_[1]->logs();
	wl2 = wls2.getLog( lognms.get( 0 ) );
    }

    BufferStringSet wnms;
    selGroup()->getChosen( wnms );
    if ( wnms.size() == 1 )
	(void)uiWellLogDispDlg::popupNonModal( this, wl1, wl2,
					       wnms.get(0), 0 );
    else if ( wnms.size() == 2 )
	(void)uiWellLogDispDlg::popupNonModal( this, wl1, wl2,
					       wnms.get(0), wnms.get(1) );
}


void uiWellMan::renameLogPush( CallBacker* )
{
    mEnsureLogSelected(uiStrings::sNoLogSel());
    BufferString lognm = logsfld_->getText();
    const uiString titl = uiStrings::phrRename(toUiString("'%1'").arg(lognm));
    uiGenInputDlg dlg( this, titl, mJoinUiStrs(sNew(),sName().toLower()),
				new StringInpSpec(lognm));
    if ( !dlg.go() )
	return;

    BufferString newnm = dlg.text();
    if ( logsfld_->isPresent(newnm) )
	mErrRet(tr("Name already in use"))

    for ( int idwell=0; idwell<currdrs_.size(); idwell++ )
    {
	currdrs_[idwell]->getLogs();
	Well::Log* log = curwds_[idwell]->logs().getLog(lognm);
	if ( log ) log->setName( newnm );
    }
    writeLogs();
}


void uiWellMan::removeLogPush( CallBacker* )
{
    mEnsureLogSelected(uiStrings::sNoLogSel());

    uiString msg;
    msg = tr("%1will be removed from disk.\nDo you wish to continue?")
	.arg(nrsel == 1 ? tr("This log ") : tr("These logs "));
    if ( !uiMSG().askRemove(msg) )
	return;

    BufferStringSet logs2rem; logsfld_->getChosen( logs2rem );

    for ( int idwell=0; idwell<currdrs_.size(); idwell++ )
    {
	currdrs_[idwell]->getLogs();
	Well::LogSet& wls = curwds_[idwell]->logs();
	for ( int idrem=0; idrem<logs2rem.size(); idrem++ )
	{
	    BufferString logname( logs2rem.get( idrem ) );
	    const Well::Log* log = wls.getLog( logname );
	    if ( log )
		delete wls.remove( wls.indexOf( logname ) );
	}
    }
    writeLogs();
}


void uiWellMan::exportLogs( CallBacker* )
{
    mEnsureLogSelected(uiStrings::sNoLogSel());

    BufferStringSet sellogs; logsfld_->getChosen( sellogs );

    for ( int idwell=0; idwell<currdrs_.size(); idwell++ )
    {
	currdrs_[idwell]->getLogs();
	currdrs_[idwell]->getD2T();
	const IOObj* obj = IOM().get( curmultiids_[idwell] );
	if ( obj ) curwds_[idwell]->info().setName( obj->name() );
    }

    uiExportLogs dlg( this, curwds_, sellogs );
    dlg.go();

    for ( int idw=0; idw<curwds_.size(); idw++ )
	{ mDeleteLogs(idw); getBasicInfo( currdrs_[idw] ); }
}


#define mAddWellInfo(key,str) \
    if ( !str.isEmpty() ) \
    { txt.add( key.getFullString() ).add( colonstr ).add( str ).addNewLine(); }


void uiWellMan::mkFileInfo()
{
    if ( !curioobj_ )
	{ setInfo( "" ); return; }

    RefMan<Well::Data> curwd = new Well::Data( curioobj_->name() );
    const Well::Reader currdr( *curioobj_, *curwd );
    BufferString txt;

    if ( currdr.getTrack() && currdr.getInfo() )
    {
	const Well::Info& info = curwd->info();
	const Well::Track& track = curwd->track();

	FixedString colonstr( ": " );
	const BufferString posstr( info.surfacecoord.toPrettyString(2), " ",
		    SI().transform(info.surfacecoord).toString() );
	mAddWellInfo(Well::Info::sCoord(),posstr)

	if ( !track.isEmpty() )
	{
	    const float rdelev = track.getKbElev();
	    const UnitOfMeasure* zun = UnitOfMeasure::surveyDefDepthUnit();
	    if ( !mIsZero(rdelev,1e-4) && !mIsUdf(rdelev) )
	    {
		txt.add(Well::Info::sKeyKBElev()).add(colonstr);
		txt.add( zun ? zun->userValue(rdelev) : rdelev, 2 );
		if ( zun ) txt.add( zun->symbol() );
		txt.addNewLine();
	    }

	    const float td = track.dahRange().stop;
	    if ( !mIsZero(td,1e-3f) && !mIsUdf(td) )
	    {
		txt.add(Well::Info::sKeyTD()).add( colonstr );
		txt.add( zun ? zun->userValue(td) : td, 2 );
		if ( zun ) txt.add( zun->symbol() );
		txt.addNewLine();
	    }

	    const double srd = SI().seismicReferenceDatum();
	    if ( !mIsZero(srd,1e-4) )
	    {
		txt.add( SurveyInfo::sKeySeismicRefDatum() ).add( colonstr );
		txt.add( zun ? zun->userValue(srd) : srd, 2 );
		if ( zun ) txt.add( zun->symbol() );
		txt.addNewLine();
	    }

	    const float replvel = info.replvel;
	    if ( !mIsUdf(replvel) )
	    {
		 txt.add(Well::Info::sKeyReplVel()).add(colonstr);
		 txt.add( zun ? zun->userValue(replvel) : replvel );
		 txt.add( UnitOfMeasure::surveyDefVelUnitAnnot(true,false)
			  .getFullString() );
		 txt.addNewLine();
	    }

	    const float groundelev = info.groundelev;
	    if ( !mIsUdf(groundelev) )
	    {
		txt.add(Well::Info::sKeyGroundElev()).add(colonstr);
		txt.add( zun ? zun->userValue(groundelev) : groundelev, 2 );
		if ( zun ) txt.add( zun->symbol() );
		txt.addNewLine();
	    }
	}

	mAddWellInfo(Well::Info::sUwid(),info.uwid)
	mAddWellInfo(Well::Info::sOper(),info.oper)
	mAddWellInfo(Well::Info::sState(),info.state)
	mAddWellInfo(Well::Info::sCounty(),info.county)

	if ( txt.isEmpty() )
	    txt.set( "<No specific info available>\n" );

    } // if ( currdr.getInfo() )

    txt.add( getFileInfo() );
    setInfo( txt );
}


const BufferStringSet& uiWellMan::getAvailableLogs() const
{
    return availablelognms_;
}