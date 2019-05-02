/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        Raman K Singh
 Date:          May 2009
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "uiclusterjobprov.h"

#include "uifileinput.h"
#include "uilabel.h"
#include "uimsg.h"
#include "uitaskrunner.h"

#include "dirlist.h"
#include "envvars.h"
#include "executor.h"
#include "file.h"
#include "filepath.h"
#include "hostdata.h"
#include "ioman.h"
#include "iostrm.h"
#include "iopar.h"
#include "jobdescprov.h"
#include "keystrs.h"
#include "oddirs.h"
#include "statrand.h"
#include "od_ostream.h"
#include "oscommand.h"
#include "settings.h"
#include "transl.h"
#include "od_helpids.h"


const char* uiClusterJobProv::sKeySeisOutIDKey()
{ return "Output Seismics Key"; }

const char* uiClusterJobProv::sKeyOutputID()
{ return "Output.0.Seismic.ID"; }


static BufferString getDefTempStorDir()
{
    BufferString stordir = "Proc_";
    stordir += HostData::localHostName();
    stordir += "_";
    stordir += Stats::randGen().getIndex(100000);
    const FilePath fp( GetDataDir(), "Seismics", stordir );
    if ( !File::createDir(fp.fullPath()) )
	return BufferString(File::getTempPath());

    return fp.fullPath();
}


class ClusterJobCreator : public Executor
{ mODTextTranslationClass(ClusterJobCreator);
public:
ClusterJobCreator( const InlineSplitJobDescProv& jobprov, const char* dir,
		   const char* prognm )
    : Executor("Job generator")
    , jobprov_(jobprov),dirnm_(dir),prognm_(prognm)
    , curidx_(0)
{
    FilePath fp( dirnm_.buf() ); fp.add( "X" );
    DirList dl( dirnm_.buf(), DirList::FilesOnly );
    for ( int idx=0; idx<dl.size(); idx++ )
    {
	fp.setFileName( dl.get(idx) );
	File::remove( fp.fullPath() );
    }
}

od_int64 nrDone() const
{ return curidx_; }

uiString uiNrDoneText() const
{ return tr("Nr jobs created"); }

od_int64 totalNr() const
{ return jobprov_.nrJobs(); }



#define mSetEnvVar(s) \
{ \
    const char* envval = GetEnvVar( s ); \
    strm << "setenv " << s << " " << (envval ? envval : "") << od_endl; \
}

static bool writeScriptFile( const char* scrfnm, const char* prognm,
			     const char* desc )
{
    od_ostream strm( scrfnm );
    if ( !strm.isOK() )
	return false;

    strm << "#!/bin/csh -f " << od_endl;

    strm << "setenv DTECT_DATA " << GetBaseDataDir() << od_endl;
    mSetEnvVar("LD_LIBRARY_PATH")
    mSetEnvVar("OD_APPL_PLUGIN_DIR")
    mSetEnvVar("OD_USER_PLUGIN_DIR")
    strm << GetExecScript(false) << " " << prognm << " \\" << od_endl;
    FilePath fp( scrfnm );
    fp.setExtension( ".par" );
    strm << fp.fullPath().buf() << od_endl;
    strm << "set exitcode = $status" << od_endl;
    strm << "echo \""; strm << desc;
    strm << " finished with code ${exitcode}\" >> \\" << od_endl;
    fp.setExtension( ".log" );
    strm << fp.fullPath().buf() << od_endl;
    strm << "exit ${exitcode}" << od_endl;
    strm.close();
    File::setPermissions( scrfnm, "711", 0 );
    return true;
}


int nextStep()
{
    if ( curidx_ >= totalNr() )
	return Finished();

    IOPar iop;
    jobprov_.getJob( curidx_, iop );
    BufferString desc( "In-line " );
    desc.add( jobprov_.objName(curidx_++) );
    iop.set( sKey::Desc(), desc.buf() );
    BufferString filenm( "Job" );
    filenm += curidx_;
    FilePath fp( dirnm_.buf() );
    fp.add( filenm );
    fp.setExtension( "par" );
    BufferString parfnm = fp.fullPath();
    fp.setExtension( "log" );
    BufferString logfnm = fp.fullPath();
    iop.set( sKey::LogFile(), logfnm );
    iop.set( sKey::DataRoot(), GetBaseDataDir() );
    iop.set( sKey::Survey(), IOM().surveyName() );
    if ( !iop.write(parfnm.buf(),sKey::Pars()) )
	return ErrorOccurred();

    fp.setExtension( "scr" );
    BufferString scrfnm = fp.fullPath();
    if ( !writeScriptFile(scrfnm.buf(),prognm_.buf(),desc.buf()))
	return ErrorOccurred();

    return MoreToDo();
}

protected:

	const InlineSplitJobDescProv&	jobprov_;
	BufferString			dirnm_;
	BufferString			prognm_;
	int				curidx_;

};


static const char* sKeyClusterProcCommand()
{ return "dTect.Cluster Proc Command"; }


uiClusterJobProv::uiClusterJobProv( uiParent* p, const IOPar& iop,
				    const char* prognm, const char* parfnm )
    : uiDialog(p,uiDialog::Setup(tr("Cluster job generator"),
				 uiString::emptyString(),
				 mODHelpKey(mClusterJobProvHelpID))
			.oktext(uiStrings::sContinue()))
    , prognm_(prognm)
    , tempstordir_(getDefTempStorDir())
    , iopar_(*new IOPar(iop))
{
    jobprov_ = new InlineSplitJobDescProv( iop );

    const int nrinl = InlineSplitJobDescProv::defaultNrInlPerJob();
    nrinlfld_ = new uiGenInput( this, tr("Nr of inlines per job"),
				IntInpSpec(nrinl) );
    nrinlfld_->valuechanging.notify( mCB(this,uiClusterJobProv,nrJobsCB) );

    nrjobsfld_ = new uiLabel( this, tr("Total no. of jobs: 0000") );
    nrjobsfld_->attach( alignedBelow, nrinlfld_ );

    parfilefld_ = new uiFileInput( this, uiStrings::sParFile(),
		    uiFileInput::Setup(uiFileDialog::Gen,parfnm)
		    .forread(false).filter("*.par;;").confirmoverwrite(false) );
    parfilefld_->attach( alignedBelow, nrjobsfld_ );

    tmpstordirfld_ = new uiFileInput( this, uiStrings::phrJoinStrings(
				    tr("Temporary"), uiStrings::sStorageDir()),
				    tempstordir_.buf() );
    tmpstordirfld_->setSelectMode( uiFileDialog::DirectoryOnly );
    tmpstordirfld_->attach( alignedBelow, parfilefld_ );

    FilePath fp( parfnm );
    fp.setExtension( 0 );
    BufferString filenm = fp.fileName();
    filenm += "_scriptdir";
    fp.setFileName( filenm.buf() );
    if ( !File::isDirectory(fp.fullPath()) )
	File::createDir( fp.fullPath() );
    scriptdirfld_ = new uiFileInput( this, uiStrings::phrStorageDir(
				     tr("for scripts")), fp.fullPath() );
    scriptdirfld_->setSelectMode( uiFileDialog::DirectoryOnly );
    scriptdirfld_->attach( alignedBelow, tmpstordirfld_ );

    BufferString cmd = "qsub";
    if ( !Settings::common().get(sKeyClusterProcCommand(),cmd) )
	Settings::common().get( "Queue.Submit", cmd );

    cmdfld_ = new uiGenInput( this, tr("Cluster Processing command"),
			      StringInpSpec(cmd) );
    cmdfld_->attach( alignedBelow, scriptdirfld_ );

    postFinalise().notify( mCB(this,uiClusterJobProv,nrJobsCB) );
}


uiClusterJobProv::~uiClusterJobProv()
{
    delete jobprov_; delete &iopar_;
}


void uiClusterJobProv::nrJobsCB( CallBacker* )
{
    jobprov_->setNrInlsPerJob( nrinlfld_->getIntValue() );
    uiString lbltxt = tr("Total no. of jobs: %1").arg(jobprov_->nrJobs());
    nrjobsfld_->setText( lbltxt );
    return;
}


#define mErrRet(s) { uiMSG().error(s); return false; }
bool uiClusterJobProv::acceptOK( CallBacker* )
{
    const int nrinlperjob = nrinlfld_->getIntValue();
    if ( mIsUdf(nrinlperjob) || nrinlperjob < 1 )
	mErrRet( tr("Please specify number of inlines per job"))

    BufferString parfnm = parfilefld_->fileName();
    if ( parfnm.isEmpty() )
	mErrRet( tr("Please enter a valid par file name"))

    BufferString tmpdir = tmpstordirfld_->fileName();
    if ( tmpdir.isEmpty() || !File::isDirectory(tmpdir) )
	mErrRet(tr("Please make a valid entry for temporary storage directory"))

    BufferString scriptdir = scriptdirfld_->fileName();
    if ( scriptdir.isEmpty() || !File::isDirectory(scriptdir) )
	mErrRet( tr("Please make a valid entry for script storage directory"))

    if ( tempstordir_ != tmpdir )
	File::remove( tempstordir_.buf() );

    const MultiID tmpid = getTmpID( tmpdir.buf() );
    MultiID outseisid;
    iopar_.get( getOutPutIDKey(), outseisid );
    iopar_.set( getOutPutIDKey(), tmpid );
    delete jobprov_;
    jobprov_ = new InlineSplitJobDescProv( iopar_ );
    jobprov_->setNrInlsPerJob( nrinlperjob );
    iopar_.set( "Output.ID", outseisid );
    iopar_.set( "Script dir", scriptdir.buf() );
    iopar_.set( sKey::TmpStor(), tmpdir.buf() );
    const char* cmd = cmdfld_->text();
    if ( !cmd || !*cmd )
	mErrRet(tr("Please enter a valid command for submitting jobs"))

    Settings& setts = Settings::common();
    setts.set( sKeyClusterProcCommand(), cmd );
    setts.write();

    iopar_.set( "Command", cmd );
    if ( !iopar_.write(parfnm.buf(),sKey::Pars()) )
	mErrRet(tr("Failed to write parameter file"))

    if ( !createJobScripts(scriptdir.buf()) )
	mErrRet(tr("Failed to split jobs"))

    uiString msg = tr("Job scripts "
		      "have been created successfully. Execute now?");
    if ( uiMSG().askGoOn(msg) )
    {
	BufferString comm( "@" );
	comm += GetExecScript( false );
	comm += " "; comm += "od_ClusterProc";
	comm += " --dosubmit "; comm += parfnm;
	if ( !OS::ExecCommand(comm,OS::RunInBG) )
	    { uiMSG().error( uiStrings::phrCannotStart(
			     uiStrings::sBatchProgram()) ); return false; }
    }

    return true;
}


bool uiClusterJobProv::createJobScripts( const char* scriptdir )
{
    if ( !jobprov_ || !jobprov_->nrJobs() )
	mErrRet(tr("No jobs to generate"))

    ClusterJobCreator exec( *jobprov_, scriptdir, prognm_ );
    uiTaskRunner dlg( this );
    return TaskRunner::execute( &dlg, exec);
}


const char* uiClusterJobProv::getOutPutIDKey() const
{
    FixedString res = iopar_.find( sKeySeisOutIDKey() );
    if ( !res )
	res = sKeyOutputID();

    return res;
}


MultiID uiClusterJobProv::getTmpID( const char* tmpdir ) const
{
    CtxtIOObj ctio( IOObjContext(&TranslatorGroup::getGroup("Seismic Data")) );
    ctio.ctxt_.stdseltype_ = IOObjContext::Seis;
    FilePath fp( tmpdir );
    BufferString objnm( "~" );
    objnm += fp.fileName();
    ctio.setName( objnm );
    IOM().to( ctio.ctxt_.getSelKey() );
    IOM().getEntry( ctio );
    if ( !ctio.ioobj_ )
	return MultiID(-1);

    fp.add( "i.*");
    MultiID ret = ctio.ioobj_->key();
    mDynamicCastGet(IOStream*,iostrm,ctio.ioobj_)
    if ( !iostrm ) return MultiID(-1);

    StepInterval<int> fnrs;
    jobprov_->getRange( fnrs );
    iostrm->fileSpec().setFileName( fp.fullPath() );
    iostrm->fileSpec().nrs_ = fnrs;
    IOM().commitChanges( *iostrm );
    return ret;
}


bool Batch::SimpleClusterProgDef::isSuitedFor( const char* pnm ) const
{
    FixedString prognm = pnm;
    return prognm == Batch::JobSpec::progNameFor( Batch::JobSpec::Attrib )
	|| prognm == Batch::JobSpec::progNameFor( Batch::JobSpec::AttribEM );
}


bool Batch::SimpleClusterProgDef::canHandle( const JobSpec& js ) const
{
    if ( !isSuitedFor(js.prognm_) )
	return false;

    FixedString outtyp = js.pars_.find(
		IOPar::compKey(sKey::Output(),sKey::Type()) );
    return outtyp != sKey::Surface();
}


uiClusterJobDispatcherLauncher::uiClusterJobDispatcherLauncher(
						Batch::JobSpec& js )
    : uiBatchJobDispatcherLauncher(js)
    , jd_(*new Batch::ClusterJobDispatcher)
{}

uiClusterJobDispatcherLauncher::~uiClusterJobDispatcherLauncher()
{ delete &jd_; }

Batch::JobDispatcher& uiClusterJobDispatcherLauncher::gtDsptchr()
{ return jd_; }

bool uiClusterJobDispatcherLauncher::go( uiParent* p )
{
    jobspec_.pars_.set( sKey::DataRoot(), GetBaseDataDir() );
    jobspec_.pars_.set( sKey::Survey(), IOM().surveyName() );
    uiClusterJobProv dlg( p, jobspec_.pars_, jobspec_.prognm_,
			  jd_.parfnm_ );
    return dlg.go();
}
