/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        A.H. Lammertink
 Date:          Jan 2004
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "uicrdevenv.h"

#include "uidesktopservices.h"
#include "uifileinput.h"
#include "uimain.h"
#include "uimsg.h"

#include "envvars.h"
#include "file.h"
#include "filepath.h"
#include "ioman.h"
#include "oddirs.h"
#include "settings.h"
#include "oscommand.h"
#include "od_helpids.h"

#ifdef __win__
# include "winutils.h"
#endif

static void showProgrDoc()
{
    const FilePath fp( mGetProgrammerDocDir(),
			__iswin__ ? "windows.html" : "unix.html" );
    uiDesktopServices::openUrl( fp.fullPath() );
}

#undef mHelpFile


uiCrDevEnv::uiCrDevEnv( uiParent* p, const char* basedirnm,
			const char* workdirnm )
	: uiDialog(p,uiDialog::Setup(tr("Create Work Enviroment"),
				     tr("Specify a work directory"),
				     mODHelpKey(mSetDataDirHelpID) ))
	, workdirfld(0)
	, basedirfld(0)
{
    const uiString titltxt = 
	tr("For OpendTect development you'll need a %1 dir\n"
        "Please specify where this directory should be created.")
	.arg(toUiString("$WORK"));

    setTitleText( titltxt );

    basedirfld = new uiFileInput( this, uiStrings::phrJoinStrings(
			      tr("Parent"),uiStrings::sDirectory()),
			      uiFileInput::Setup(basedirnm).directories(true) );

    workdirfld = new uiGenInput( this, mJoinUiStrs(sDirectory(),sName()),
								    workdirnm );
    workdirfld->attach( alignedBelow, basedirfld );

}


bool uiCrDevEnv::isOK( const char* datadir )
{
#ifdef __mac__
    FilePath datafp( datadir, "Resources" );
#else
    FilePath datafp( datadir );
#endif

    if ( !datafp.nrLevels() ) return false;

    if ( !datafp.nrLevels() || !File::isDirectory( datafp.fullPath() ) )
	return false;

    datafp.add( "CMakeLists.txt" );
    if ( !File::exists(datafp.fullPath()) )
	return false;

    return true;
}


#undef mErrRet
#define mErrRet(s) { uiMSG().error(s); return; }

void uiCrDevEnv::crDevEnv( uiParent* appl )
{
    BufferString swdir = GetSoftwareDir(0);
    if ( !isOK(swdir) )
    {
	uiMSG().error(tr("No source code found. Please download\n"
			 "and install development package first"));
	return;
    }

    FilePath oldworkdir( GetEnvVar("WORK") );
    const bool oldok = isOK( oldworkdir.fullPath() );

    BufferString workdirnm;

    if ( File::exists(oldworkdir.fullPath()) )
    {
	uiString msg = tr("Your current work directory (%1) %2 to be "
			  "a valid work directory."
			  "\n\nDo you want to completely remove "
			  "the existing directory "
			  "and create a new work directory there?")
		     .arg(oldworkdir.fullPath())
		     .arg(oldok ? tr("seems")
				: tr("does not seem"));

	if ( uiMSG().askGoOn(msg) )
	{
	    File::remove( workdirnm );
	    workdirnm = oldworkdir.fullPath();
	}
    }

    if ( workdirnm.isEmpty() )
    {
	BufferString worksubdirm = "ODWork";
	BufferString basedirnm = GetPersonalDir();

	// pop dialog
	uiCrDevEnv dlg( appl, basedirnm, worksubdirm );
	if ( !dlg.go() ) return;

	basedirnm = dlg.basedirfld->text();
	worksubdirm = dlg.workdirfld->text();

	if ( !File::isDirectory(basedirnm) )
	    mErrRet(tr("Invalid directory selected"))

	workdirnm = FilePath( basedirnm ).add( worksubdirm ).fullPath();
    }

    if ( workdirnm.isEmpty() ) return;

    if ( File::exists(workdirnm) )
    {
	uiString msg;
	const bool isdir= File::isDirectory( workdirnm );

	if ( isdir )
	{
	    msg = tr("The directory you selected (%1)\nalready exists.\n\n")
		.arg(workdirnm);
	}
	else
	{
	    msg = tr("You selected a file.\n\n");
	}

	msg.arg("Do you want to completely remove the existing %1"
		"and create a new work directory there?")
	    .arg(isdir ? tr("directory\n") : tr("file\n"));

	if ( !uiMSG().askRemove(msg) )
	    return;

	File::remove( workdirnm );
    }


    if ( !File::createDir( workdirnm ) )
	mErrRet( uiStrings::phrCannotCreateDirectory(toUiString(workdirnm)) )

    const uiString docmsg =
      tr("The OpendTect window will FREEZE during this process\n"
      "- for upto a few minutes.\n\n"
      "Meanwhile, do you want to take a look at the developers documentation?");
    
    if ( uiMSG().askGoOn(docmsg) )
	showProgrDoc();

#ifdef __mac__
    FilePath fp( swdir, "Resources" );
#else
    FilePath fp( swdir, "bin" );
#endif

#ifdef __win__
    BufferString cmd;
    fp.add( "od_cr_dev_env.bat" );
    cmd += fp.fullPath();
    cmd += " "; cmd += swdir;
    char shortpath[1024];
    GetShortPathName(workdirnm.buf(),shortpath,1024);
    cmd += " "; cmd += shortpath;

    OS::CommandExecPars execpars( false );
    execpars.launchtype( OS::Wait4Finish )
	    .isconsoleuiprog( true );
    OS::MachineCommand mc( cmd );
    OS::CommandLauncher cl( mc );
    cl.execute( execpars );
#else
    fp.add( "od_cr_dev_env" );
    BufferString cmd( "'", fp.fullPath() );
    cmd += "' '"; cmd += swdir;
#ifdef __mac__
    cmd += "/Resources";
#endif

    cmd += "' '"; cmd += workdirnm; cmd += "'";
    system( cmd );
#endif

    BufferString cmakefile =
			FilePath(workdirnm).add("CMakeLists.txt").fullPath();
    if ( !File::exists(cmakefile) )
	mErrRet(tr("Creation seems to have failed"))
    else
	uiMSG().message( tr("Creation seems to have succeeded.") );
}



#undef mErrRet
#define mErrRet(msg) { uiMSG().error( msg ); return false; }

bool uiCrDevEnv::acceptOK( CallBacker* )
{
    BufferString workdir = basedirfld->text();
    if ( workdir.isEmpty() || !File::isDirectory(workdir) )
	mErrRet( tr("Please enter a valid (existing) location") )

    if ( workdirfld )
    {
	BufferString workdirnm = workdirfld->text();
	if ( workdirnm.isEmpty() )
	    mErrRet( tr("Please enter a (sub-)directory name") )

	workdir = FilePath( workdir ).add( workdirnm ).fullPath();
    }

    if ( !File::exists(workdir) )
    {
#ifdef __win__
	if ( workdir.contains( "Program Files" )
	  || workdir.contains( "program files" )
	  || workdir.contains( "PROGRAM FILES" ) )
	  mErrRet(tr("Please do not use 'Program Files'.\n"
		     "Instead, a directory like 'My Documents' would be OK."))
#endif
    }

    return true;
}
