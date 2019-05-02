/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	Nanne Hemstra
 Date:		August 2014
________________________________________________________________________

-*/

static const char* rcsID mUsedVar = "$Id$";

#include "uiimpexp2dgeom.h"

#include "ui2dgeomman.h"
#include "uifileinput.h"
#include "uigeninput.h"
#include "uigeom2dsel.h"
#include "uiioobjselgrp.h"
#include "uimsg.h"
#include "uitblimpexpdatasel.h"

#include "geom2dascio.h"
#include "od_iostream.h"
#include "posinfo2d.h"
#include "survgeometrytransl.h"
#include "survgeom2d.h"
#include "tabledef.h"


// uiImp2DGeom
uiImp2DGeom::uiImp2DGeom( uiParent* p, const char* lnm )
    : uiDialog(p,uiDialog::Setup(tr("Import New Line Geometry"),
				 mNoDlgTitle,
				 mODHelpKey(mGeom2DImpDlgHelpID)))
    , singlemultifld_(0)
    , linefld_(0)
    , linenm_(lnm)
    , geomfd_(0)
{
    const FixedString linenm( lnm );
    const bool lineknown = !linenm.isEmpty();
    if ( lineknown )
    {
	uiString title = tr("Set new geometry for %1").arg( linenm );
	setTitleText( title );
    }

    setOkCancelText( uiStrings::sImport(), uiStrings::sClose() );

    geomfd_ = Geom2dAscIO::getDesc( linenm.isEmpty() );
    fnmfld_ = new uiFileInput( this, tr("Input Geometry File"),
				   uiFileInput::Setup().withexamine(true) );
    uiObject* attachobj = fnmfld_->attachObj();
    if ( !lineknown )
    {
	singlemultifld_ = new uiGenInput( this, tr("File contains geometry for"),
		    BoolInpSpec(false,tr("Single line"),tr("Multiple lines")) );
	singlemultifld_->attach( alignedBelow, fnmfld_ );
	mAttachCB( singlemultifld_->valuechanged, uiImp2DGeom::singmultCB );
	attachobj = singlemultifld_->attachObj();

	linefld_ = new uiGeom2DSel( this, false );
    }

    dataselfld_ = new uiTableImpDataSel( this, *geomfd_, mNoHelpKey );
    dataselfld_->attach( alignedBelow, attachobj );

    if ( linefld_ )
	linefld_->attach( alignedBelow, dataselfld_ );
}


uiImp2DGeom::~uiImp2DGeom()
{
    detachAllNotifiers();
    delete geomfd_;
}


void uiImp2DGeom::singmultCB( CallBacker* )
{
    const bool singleline = singlemultifld_->getBoolValue();
    linefld_->display( singleline );
    Geom2dAscIO::fillDesc( *geomfd_, !singleline );
}


bool uiImp2DGeom::acceptOK( CallBacker* )
{
    if ( File::isEmpty(fnmfld_->fileName()) )
    { uiMSG().error(uiStrings::sInvInpFile()); return false; }

    if ( !linenm_.isEmpty() )
	return false;

    if ( singlemultifld_->getBoolValue() )
    {
	const IOObj* ioobj = linefld_->ioobj();
	if ( !ioobj )
	    return false;

	const BufferString linenm = ioobj->name();
	const Pos::GeomID geomid = Geom2DImpHandler::getGeomID( linenm );
	if ( geomid == mUdfGeomID )
	    return false;

	RefMan<Survey::Geometry2D> geom2d;
	mDynamicCast(Survey::Geometry2D*,geom2d,
		     Survey::GMAdmin().getGeometry(geomid))
	if ( !fillGeom(*geom2d) )
	    return false;

	uiString errmsg;
	if ( !Survey::GMAdmin().write(*geom2d,errmsg) )
	{
	    uiMSG().error( errmsg );
	    return false;
	}

	uiMSG().message( tr("Line %1 successfully imported.").arg(linenm) );
    }
    else
    {
	ObjectSet<Survey::Geometry2D> geoms;
	if ( !fillGeom(geoms) )
	    return false;

	BufferStringSet linenms;
	for ( int idx=0; idx<geoms.size(); idx++ )
	    linenms.add( geoms[idx]->getName() );

	TypeSet<Pos::GeomID> geomids;
	if ( !Geom2DImpHandler::getGeomIDs(linenms,geomids) )
	    return false;

	uiStringSet errors;
	for ( int idx=0; idx<geoms.size(); idx++ )
	{
	    uiString errmsg;
	    if ( !Survey::GMAdmin().write(*geoms[idx],errmsg) )
		errors.add( errmsg );
	}

	if ( !errors.isEmpty() )
	{
	    uiMSG().errorWithDetails( errors, tr("Error during import:") );
	    return false;
	}

	uiMSG().message( tr("Lines successfully imported.") );
    }

    return false;
}


od_istream* uiImp2DGeom::getStrm() const
{
     BufferString filenm( fnmfld_->fileName() );
    if ( filenm.isEmpty() )
	return 0;

    od_istream* strm = new od_istream( filenm );
    if ( !strm->isOK() )
    {
	uiMSG().error( uiStrings::sCantOpenInpFile() );
	delete strm;
	return 0;
    }

    return strm;
}


bool uiImp2DGeom::fillGeom( ObjectSet<Survey::Geometry2D>& geoms )
{
    PtrMan<od_istream> strm = getStrm();
    Geom2dAscIO geomascio( dataselfld_->desc(), *strm );
    if ( !geomascio.getData(geoms) )
    {
	uiMSG().error( geomascio.errMsg() );
	return false;
    }

    return true;
}


bool uiImp2DGeom::fillGeom( Survey::Geometry2D& geom )
{
    PtrMan<od_istream> strm = getStrm();
    Geom2dAscIO geomascio( dataselfld_->desc(), *strm );
    if ( !geomascio.getData(geom) )
    {
	uiMSG().error( geomascio.errMsg() );
	return false;
    }

    return true;
}


// uiExp2DGeom
uiExp2DGeom::uiExp2DGeom( uiParent* p )
    : uiDialog(p,Setup(uiStrings::phrExport( tr("2D Geometry")),mNoDlgTitle,
		       mODHelpKey(mExp2DGeomHelpID)).modal(false))
{
    setOkCancelText( uiStrings::sExport(), uiStrings::sClose() );

    IOObjContext ctxt = mIOObjContext( SurvGeom2D );
    geomfld_ = new uiIOObjSelGrp( this, ctxt,
				  uiIOObjSelGrp::Setup(OD::ChooseAtLeastOne) );

    outfld_ = new uiFileInput( this, uiStrings::sOutputFile(),
			       uiFileInput::Setup().forread(false) );
    outfld_->attach( alignedBelow, geomfld_ );
}


uiExp2DGeom::~uiExp2DGeom()
{
}


bool uiExp2DGeom::acceptOK( CallBacker* )
{
    const BufferString fnm = outfld_->fileName();
    if ( fnm.isEmpty() )
    {
	uiMSG().error( tr("Please enter the output file name") );
	return false;
    }

    od_ostream strm( fnm );
    if ( !strm.isOK() )
    {
	uiMSG().error( tr("Cannot open the output file") );
	return false;
    }

    BufferString outstr;
    TypeSet<MultiID> mids; geomfld_->getChosen( mids );
    for ( int gidx=0; gidx<mids.size(); gidx++ )
    {
	const Survey::Geometry* geom = Survey::GM().getGeometry( mids[gidx] );
	mDynamicCastGet(const Survey::Geometry2D*,geom2d,geom)
	if ( !geom2d ) continue;

	const PosInfo::Line2DData& data2d = geom2d->data();
	const TypeSet<PosInfo::Line2DPos>& allpos = data2d.positions();

	for ( int pidx=0; pidx<allpos.size(); pidx++ )
	{
	    const PosInfo::Line2DPos& pos = allpos[pidx];
	    outstr.setEmpty();
	    const BufferString xcrdstr = toString(pos.coord_.x,2);
	    const BufferString ycrdstr = toString(pos.coord_.y,2);
	    outstr.add( data2d.lineName() ).addTab()
		  .add( pos.nr_ ).addTab()
		  .add( xcrdstr.buf() ).addTab()
		  .add( ycrdstr.buf() );
	    strm << outstr.buf() << '\n';
	}
    }

    strm.close();

    const uiString msg = tr("Geometry successfully exported.\n\n"
			    "Do you want to export more?");
    const bool res = uiMSG().askGoOn( msg );
    return !res;
}