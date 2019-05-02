/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	Bert
 Date:		Dec 2011
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "svnaccess.h"
#include "filepath.h"
#include "file.h"
#include "genc.h"
#include "od_iostream.h"
#include "oscommand.h"
#include "dirlist.h"

static const char* sRedirect = " > /dev/null 2>&1";

#define mGetReqFnm() const BufferString reqfnm( FilePath(dir_,fnm).fullPath() )

#define mGetTmpFnm(op,fnm) \
    const BufferString tmpfnm( \
	FilePath(File::getTempPath(),BufferString(fnm,GetPID(),op)).fullPath() )

#define mRetRmTempFile() \
{ if ( File::exists(tmpfnm) ) File::remove(tmpfnm); return; }


SVNAccess::SVNAccess( const char* dir )
    : dir_(dir)
    , havesvn_(File::exists( FilePath(dir,".svn").fullPath()) )
{
    if ( !havesvn_ ) return;

    const BufferString fnm( FilePath(dir,".svn","entries").fullPath() );
    od_istream strm( FilePath(dir,".svn","entries") );
    if ( !strm.isOK() )

    for ( int idx=0; idx<5; idx++ )
	if ( !strm.skipLine() )
	    return;

    BufferString reposinfo;
    if ( strm.getLine(reposinfo) )
    {
	char* reposnm = reposinfo.find( '/' );
	if ( reposnm ) *reposnm++ = '\0';
	const_cast<BufferString&>(reposdir_) = reposnm;
	const char* hostnm = firstOcc( reposinfo.buf(), "//" );
	if ( hostnm ) hostnm++;
	const_cast<BufferString&>(host_) = hostnm;
    }
}


SVNAccess::~SVNAccess()
{
}


bool SVNAccess::isOK() const
{
    bool hostreachable = true;
#ifdef __win__
    //TODO
#else
    const BufferString cmd( "@ping -q -c 1 -W 2 ", host_, sRedirect );
    hostreachable = OS::ExecCommand(cmd);
#endif
    return havesvn_ && hostreachable;
}


bool SVNAccess::isInSVN( const char* fnm ) const
{
    if ( !fnm || !*fnm || !isOK() ) return false;
    FilePath fp( fnm );
    BufferString dirnm( fp.pathOnly() );
    BufferStringSet entries; getEntries( dirnm, entries );
    return entries.isPresent( fp.fileName() );
}


void SVNAccess::getEntries( const char* dir, BufferStringSet& entries ) const
{
    entries.erase();
    const FilePath fp( dir_, dir, ".svn", "text-base" );
    DirList dl( fp.fullPath(), DirList::FilesOnly );
    for ( int idx=0; idx<dl.size(); idx++ )
    {
	BufferString fnm( dl.get(idx) );
	char* ptr = fnm.find( ".svn-base" );
	if ( ptr )
	    { *ptr = '\0'; entries.add( fnm ); }
    }
}


bool SVNAccess::update( const char* fnm )
{
    if ( !isOK() )
	return true;

    mGetReqFnm();
    const BufferString cmd( "@svn update ", reqfnm, sRedirect );
    return OS::ExecCommand(cmd);
}


bool SVNAccess::lock( const char* fnm )
{
    BufferStringSet bss; bss.add( fnm );
    return lock( bss );
}


bool SVNAccess::lock( const BufferStringSet& fnms )
{
    if ( !isOK() )
	return true;

    BufferString cmd( "@svn edit " );
    for ( int idx=0; idx<fnms.size(); idx++ )
    {
	const char* fnm = fnms.get(idx).buf();
	mGetReqFnm();
	cmd.add( " \"" ).add( reqfnm ).add( "\"" );
    }
    cmd.add( sRedirect );
    return OS::ExecCommand(cmd);
}


bool SVNAccess::add( const char* fnm )
{
    BufferStringSet bss; bss.add( fnm );
    return add( bss );
}


bool SVNAccess::add( const BufferStringSet& fnms )
{
    if ( fnms.isEmpty() )
	return true;
    else if ( !isOK() )
	return false;

    BufferString cmd( "@svn add" );
    for ( int idx=0; idx<fnms.size(); idx++ )
    {
	const char* fnm = fnms.get(idx).buf();
	mGetReqFnm();
	cmd.add( " \"" ).add( reqfnm ).add( "\"" );
    }
    cmd.add( sRedirect );
    return OS::ExecCommand(cmd);
}


bool SVNAccess::remove( const char* fnm )
{
    if ( !isInSVN(fnm) )
	return File::remove(fnm);

    BufferStringSet bss; bss.add( fnm );
    return remove( bss );
}


bool SVNAccess::remove( const BufferStringSet& fnms )
{
    if ( fnms.isEmpty() )
	return true;
    const bool isok = isOK();

    BufferString cmd( "@svn delete -f" );
    for ( int idx=0; idx<fnms.size(); idx++ )
    {
	const char* fnm = fnms.get(idx).buf();
	if ( !isok )
	    File::remove( fnm );
	else
	{
	    mGetReqFnm();
	    cmd.add( " \"" ).add( reqfnm ).add( "\"" );
	}
    }
    if ( !isok )
	return true;

    cmd.add( sRedirect );
    return OS::ExecCommand(cmd);
}


bool SVNAccess::commit( const char* fnm, const char* msg )
{
    BufferStringSet bss; bss.add( fnm );
    return commit( bss, msg );
}


bool SVNAccess::commit( const BufferStringSet& fnms, const char* msg )
{
    if ( fnms.isEmpty() || !isOK() )
	return true;

    BufferString cmd( "@svn commit" );
    mGetTmpFnm("svncommit",fnms.get(0));
    bool havetmpfile = false;
    if ( !msg || !*msg )
	cmd.add( " -m \".\"" );
    else
    {
	od_ostream strm( tmpfnm );
	if ( strm.isOK() )
	{
	    strm << msg << od_endl;
	    havetmpfile = true;
	    cmd.add( " -F \"" ).add( tmpfnm ).add( "\"" );
	}
    }

    for ( int idx=0; idx<fnms.size(); idx++ )
    {
	const char* fnm = fnms.get(idx).buf();
	if ( fnm && *fnm )
	{
	    mGetReqFnm();
	    cmd.add( " \"" ).add( reqfnm ).add( "\"" );
	}
    }

    cmd.add( sRedirect );
    const bool res = OS::ExecCommand(cmd);
    if ( havetmpfile )
	File::remove( tmpfnm );
    return res;
}


bool SVNAccess::rename( const char* subdir, const char* from, const char* to )
{
    const FilePath sdfp( dir_, subdir );
    FilePath fromfp( sdfp, from ), tofp( sdfp, to );
    const BufferString fromfullfnm( fromfp.fullPath() );
    const BufferString tofullfnm( tofp.fullPath() );
    fromfp.set( subdir ); fromfp.add( from );
    tofp.set( subdir ); tofp.add( to );
    const BufferString fromfnm( fromfp.fullPath() );
    const BufferString tofnm( tofp.fullPath() );
    BufferString cmd( "@svn rename '", fromfnm, "' '" );
    cmd.add( tofnm ).add( "." ).add( sRedirect );
    return OS::ExecCommand(cmd);
}


bool SVNAccess::changeFolder( const char* fnm, const char* fromsubdir,
			      const char* tosubdir )
{
    FilePath tofp( dir_, tosubdir, fnm );
    FilePath fromfp( dir_, fromsubdir, fnm );
    const BufferString fromfullfnm( fromfp.fullPath() );
    const BufferString tofullfnm( tofp.fullPath() );
    fromfp.set( fromsubdir ); fromfp.add( fnm );
    tofp.set( tosubdir ); tofp.add( fnm );
    const BufferString fromfnm( fromfp.fullPath() );
    const BufferString tofnm( tofp.fullPath() );

    BufferString cmd( "@svn move '", fromfnm, "' '" );
    cmd.add( tofnm ).add( "." ).add( sRedirect );
    return OS::ExecCommand(cmd);
}


void SVNAccess::diff( const char* fnm, BufferString& res ) const
{
    res.setEmpty();
    if ( !isOK() )
	return;

    mGetReqFnm();

#ifdef __win__
    return; //TODO
#else

    BufferString cmd( "@svn diff ", reqfnm, " > " );
    mGetTmpFnm("svndiff",fnm);
    cmd.add( "\"" ).add( tmpfnm ).add( "\" 2> /dev/null" );
    if ( !OS::ExecCommand(cmd) )
	mRetRmTempFile()

    od_istream strm( tmpfnm );
    if ( !strm.isOK() )
	mRetRmTempFile()

    BufferString line;
    while ( strm.getLine(line) )
    {
	if ( line.isEmpty() )
	    continue;
	if ( !res.isEmpty() )
	    res.add( "\n" );
	res.add( line );
    }

    mRetRmTempFile()
#endif
}
