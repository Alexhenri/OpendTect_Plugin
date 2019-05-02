/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : Raman Singh
 * DATE     : Sept 2008
-*/

static const char* rcsID mUsedVar = "$Id$";

#include "gmtdef.h"
#include "gmtprocflow.h"
#include "gmtprocflowtr.h"
#include "ascstream.h"
#include "ioman.h"

defineTranslatorGroup(ODGMTProcFlow,"GMT process flow");
uiString ODGMTProcFlowTranslatorGroup::sTypeName(int num)
{ return tr("GMT process flow",0,num); }

defineTranslator(dgb,ODGMTProcFlow,mDGBKey);
mDefSimpleTranslatorioContextWithExtra(ODGMTProcFlow,None,
					ctxt->selkey_ = ODGMT::sKeyGMTSelKey())


ODGMT::ProcFlow::ProcFlow( const char* nm )
    : NamedObject(nm)
{
}


ODGMT::ProcFlow::~ProcFlow()
{
}


int ODGMTProcFlowTranslatorGroup::selector( const char* key )
{
    int retval = defaultSelector( sGroupName(), key );
    if ( retval ) return retval;
    return defaultSelector("GMT data",key) ? 1 : 0;
}


bool ODGMTProcFlowTranslator::retrieve( ODGMT::ProcFlow& pf, const IOObj* ioobj,
					BufferString& bs )
{
    if ( !ioobj ) { bs = "Cannot find flow object in data base"; return false; }
    mDynamicCast(ODGMTProcFlowTranslator*,PtrMan<ODGMTProcFlowTranslator> tr,
		 ioobj->createTranslator());
    if ( !tr ) { bs = "Selected object is not a GMT flow"; return false; }

    PtrMan<Conn> conn = ioobj->getConn( Conn::Read );
    if ( !conn )
        { bs = "Cannot open "; bs += ioobj->fullUserExpr(true); return false; }
    bs = tr->read( pf, *conn );
    return bs.isEmpty();
}


bool ODGMTProcFlowTranslator::store( const ODGMT::ProcFlow& pf,
				     const IOObj* ioobj, BufferString& bs )
{
    if ( !ioobj ) { bs = "No object to store flow in data base"; return false; }
    mDynamicCast(ODGMTProcFlowTranslator*,PtrMan<ODGMTProcFlowTranslator> tr,
		 ioobj->createTranslator());

    if ( !tr ) { bs = "Selected object is not a GMT flow"; return false;}

    bs = "";
    PtrMan<Conn> conn = ioobj->getConn( Conn::Write );
    if ( !conn )
        { bs = "Cannot open "; bs += ioobj->fullUserExpr(false); }
    else
	bs = tr->write( pf, *conn );

    return bs.isEmpty();
}


const char* dgbODGMTProcFlowTranslator::read( ODGMT::ProcFlow& pf, Conn& conn )
{
    if ( !conn.forRead() || !conn.isStream() )
	return "Internal error: bad connection";

    ascistream astrm( ((StreamConn&)conn).iStream() );
    if ( !astrm.isOK() )
	return "Cannot read from input file";
    if ( !astrm.isOfFileType(mTranslGroupName(ODGMTProcFlow)) )
	return "Input file is not a Processing flow";
    if ( atEndOfSection(astrm) )
	astrm.next();
    if ( atEndOfSection(astrm) )
	return "Input file is empty";

    pf.setName( IOM().nameOf(conn.linkedTo()) );
    pf.pars().getFrom( astrm );
    return 0;
}


const char* dgbODGMTProcFlowTranslator::write( const ODGMT::ProcFlow& pf,
						Conn& conn )
{
    if ( !conn.forWrite() || !conn.isStream() )
	return "Internal error: bad connection";

    ascostream astrm( ((StreamConn&)conn).oStream() );
    astrm.putHeader( mTranslGroupName(ODGMTProcFlow) );
    if ( !astrm.isOK() )
	return "Cannot write to output GMT flow file";

    pf.pars().putTo( astrm );
    return astrm.isOK() ? 0 : "Error during write to GMT flow file";
}
