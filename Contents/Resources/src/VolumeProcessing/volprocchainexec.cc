/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : K. Tingdahl
 * DATE     : October 2006
-*/

static const char* rcsID mUsedVar = "$Id$";

#include "volprocchainexec.h"

#include "jobcommunic.h"
#include "odsysmem.h"
#include "posinfo.h"
#include "seisdatapack.h"
#include "simpnumer.h" // for getCommonStepInterval
#include "threadwork.h"

#include "hiddenparam.h"


static HiddenParam<VolProc::ChainExecutor,ObjectSet<TrcKeyZSampling>* >
					    volprocchainexecstepstkzsmgr_( 0 );

uiString VolProc::ChainExecutor::sGetStepErrMsg()
{
    return uiStrings::phrCannotFind( tr("output step with id: %1") );
}



VolProc::ChainExecutor::ChainExecutor( Chain& vr )
    : ::Executor( "Volume processing" )
    , chain_( vr )
    , isok_( false )
    , outputdp_( 0 )
    , outputhrg_(false)
    , totalnrepochs_( 1 )
    , curepoch_( 0 )
    , jobcomm_( 0 )
{
    volprocchainexecstepstkzsmgr_.setParam( this,
					new ObjectSet<TrcKeyZSampling> );
    setName( vr.name().getFullString() );
    web_ = chain_.getWeb();
    //TODO Optimize connections, check for identical steps using same inputs
}


VolProc::ChainExecutor::~ChainExecutor()
{
    deepErase( *volprocchainexecstepstkzsmgr_.getParam( this ) );
    volprocchainexecstepstkzsmgr_.removeAndDeleteParam( this );

    deepErase( epochs_ );
    if ( curepoch_ )
	delete curepoch_;

    DataPackMgr& seismgr = DPM( DataPackMgr::SeisID() );
    if ( !outputdp_ || !seismgr.haveID(outputdp_->id()) )
	return;

    seismgr.release( outputdp_->id() );
}


uiString VolProc::ChainExecutor::errMsg() const
{ return errmsg_; }


bool VolProc::ChainExecutor::setCalculationScope( const TrcKeySampling& tks,
					 const StepInterval<float>& zsamp,
					 od_uint64& maxmemusage, int* nrchunks )
{
    isok_ = false;
    if ( !scheduleWork() )
	return isok_;

    const int nrchunksret = nrChunks( tks, zsamp, maxmemusage );
    if ( nrchunks )
	*nrchunks = nrchunksret;

    isok_ = nrchunksret > 0;

    return isok_;
}


void VolProc::ChainExecutor::adjustStepsNrComponents( bool is2d )
{
    for ( int iepoch=epochs_.size()-1; iepoch>=0; iepoch-- )
    {
	const ObjectSet<Step>& steps = epochs_[iepoch]->getSteps();
	for ( int istep=0; istep<steps.size(); istep++ )
	{
	    VolProc::Step* step = const_cast<VolProc::Step*>( steps[istep] );
	    const BufferString stepnm( step->factoryKeyword() );
	    if ( is2d )
	    {
		if ( stepnm == "FaultThinning" )
		{
		    step->setInpNrComps( 0, 2 );
		}

		if ( stepnm == "FaultThinning" ||
		     stepnm == "FaultLikelihood" )
		{
		    step->setOutputNrComps( 2 );
		    continue;
		}
	    }

	    if ( stepnm == "Volume Statistics" ||
		 stepnm == "DipTensorSmoother" ||
		 stepnm == "Recursive Gaussian Filter" )
	    {
		step->setOutputNrComps( step->getNrInputComponents(0) );
	    }

	    //Set good defaults for legacy steps:
	    if ( step->needsInput() )
	    {
		const int nrinp = step->getNrInputs();
		for ( int slotidx=0; slotidx<nrinp; slotidx++ )
		{
		    if ( step->getNrInputComponents(slotidx) > 0 )
			continue;

		    step->setInpNrComps( slotidx, 1 );
		}
	    }

	    if ( step->getNrOutComponents() > 0 )
		continue;

	    step->setOutputNrComps( 1 );
	}
    }
}

#define mGetStep( var, id, errret ) \
Step* var = chain_.getStepFromID( id ); \
if ( !var ) \
{ \
    errmsg_ = sGetStepErrMsg().arg(toString(id)); \
    return errret; \
}


bool VolProc::ChainExecutor::scheduleWork()
{
    deepErase( epochs_ );
    scheduledsteps_.erase();

    ObjectSet<Step> unscheduledsteps;

    //This is the output step of the whole chain,
    //but the input step of the connection since in receives data
    mGetStep( chainoutputstep, chain_.outputstepid_, false );

    unscheduledsteps += chainoutputstep;

    int firstepoch = 0;
    while ( unscheduledsteps.size() )
    {
	Step* currentstep = unscheduledsteps.pop(); //an inputstep
	if ( !scheduledsteps_.addIfNew( currentstep ) )
	    continue;

	const int lateststepepoch = computeLatestEpoch( currentstep->getID() );
	if ( mIsUdf(lateststepepoch) )
	{
	    pErrMsg("Cannot compute latest epoch");
	    return false;
	}

	firstepoch = mMAX( firstepoch, lateststepepoch );

	TypeSet<Chain::Connection> inputconnections;
	web_.getConnections( currentstep->getID(), true, inputconnections );
	for ( int idx=0; idx<inputconnections.size(); idx++ )
	{
	    mGetStep( outputstep, inputconnections[idx].outputstepid_, false );
	    if ( scheduledsteps_.isPresent( outputstep ) ||
		 unscheduledsteps.isPresent( outputstep ) )
		continue;

	    unscheduledsteps += outputstep;
	}
    }

    const int nrepochs = firstepoch+1;
    for ( int idx=0; idx<nrepochs; idx++ )
    {
	Epoch* epoch = new Epoch( *this );
	for ( int idy=0; idy<scheduledsteps_.size(); idy++ )
	    if ( computeLatestEpoch( scheduledsteps_[idy]->getID() )==idx )
		epoch->addStep( scheduledsteps_[idy] );

	epochs_ += epoch;
    }

    totalnrepochs_ = epochs_.size();

    return true;
}


void VolProc::ChainExecutor::updateScheduledStepsSampling(
					const TrcKeySampling& tks,
					const StepInterval<float>& zsamp )
{
    ObjectSet<TrcKeyZSampling>& stepstkzs =
				*volprocchainexecstepstkzsmgr_.getParam( this );
    deepErase( stepstkzs );
    for ( int idx=0; idx<chain_.getSteps().size(); idx++ )
    {
	stepstkzs += new TrcKeyZSampling;
	stepstkzs[idx]->setEmpty();
    }

    const int laststepidx = chain_.indexOf(
			    chain_.getStepFromID( chain_.outputstepid_ ) );
    stepstkzs[laststepidx]->hsamp_ = tks;
    stepstkzs[laststepidx]->zsamp_ = zsamp;
    for ( int iepoch=0; iepoch<totalnrepochs_; iepoch++ )
    {
	const ObjectSet<Step>& steps = epochs_[iepoch]->getSteps();
	for ( int istep=0; istep<steps.size(); istep++ )
	{
	    const Step* outstep = steps[istep];
	    const int outstepidx = chain_.indexOf( outstep );
	    const TrcKeyZSampling& outtkzs = *stepstkzs[outstepidx];
	    TypeSet<Chain::Connection> conns;
	    web_.getConnections( outstep->getID(), true, conns );
	    for ( int iconn=0; iconn<conns.size(); iconn++ )
	    {
		const Step::ID inpstepid = conns[iconn].outputstepid_;
		const Step* inpstep = chain_.getStepFromID( inpstepid );
		const int inpstepidx = chain_.indexOf( inpstep );
		const TrcKeyZSampling tkzs =
					   outstep->getInputSampling( outtkzs );
		if ( stepstkzs[inpstepidx]->hsamp_.isDefined() )
		    stepstkzs[inpstepidx]->include( tkzs );
		else
		    *stepstkzs[inpstepidx] = tkzs;
	    }
	}
    }
}


int VolProc::ChainExecutor::computeLatestEpoch( Step::ID stepid ) const
{
    if ( stepid==chain_.outputstepid_ )
	return 0;

    TypeSet<Chain::Connection> outputconnections;
    web_.getConnections( stepid, false, outputconnections );

    int latestepoch = mUdf(int);
    for ( int idx=0; idx<outputconnections.size(); idx++ )
    {
	const int curepoch =
	    computeLatestEpoch( outputconnections[idx].inputstepid_ );
	if ( mIsUdf(curepoch) )
	    continue;

	const int newepoch = curepoch+1;
	if ( mIsUdf(latestepoch) || newepoch>latestepoch )
	    latestepoch = newepoch;
    }

    return latestepoch;
}


void VolProc::ChainExecutor::computeComputationScope( Step::ID,
				TrcKeySampling&,StepInterval<int>&) const
{}


bool VolProc::ChainExecutor::getCalculationScope( Step::ID stepid,
						  TrcKeyZSampling& tkzs ) const
{
    const ObjectSet<TrcKeyZSampling>& stepstkzs =
				*volprocchainexecstepstkzsmgr_.getParam( this );
    const int stepidx = chain_.indexOf( chain_.getStepFromID( stepid ) );
    if ( !stepstkzs.validIdx(stepidx) )
	return false;

    tkzs = *stepstkzs[stepidx];

    return true;
}


namespace VolProc
{

struct VolumeMemory
{
    Step::ID		creator_;
    Step::OutputSlotID	outputslot_;
    od_int64		nrbytes_;
    int			epoch_;

			VolumeMemory( Step::ID creator,
				      Step::OutputSlotID outputslot,
				      od_int64 nrbytes,
				      int epoch )
			: creator_(creator)
			, outputslot_(outputslot)
			, nrbytes_(nrbytes)
			, epoch_(epoch)				{}

    bool		operator==( VolumeMemory vm ) const
			{
			    return creator_ == vm.creator_ &&
				   outputslot_ == vm.outputslot_ &&
				   nrbytes_ == vm.nrbytes_ &&
				   epoch_ == vm.epoch_;
			}
};

} // namespace VolProc

// From ValSeries:
#if defined __lux32__
#define mChunkSize	0x20000000
#elif defined __win32__
#define mChunkSize	0x20000000
#else
#define mChunkSize	0x800000000
#endif


bool VolProc::ChainExecutor::needSplit( od_int64 memusage,
					od_int64& freemem, int& nrexecs ) const
{
    od_int64 totmem;
    OD::getSystemMemory( totmem, freemem );
    if ( memusage >= freemem )
    {
	nrexecs = freemem <= 0 ? 0
			       : mNINT32( Math::Ceil( mCast(double,memusage) /
					  mCast(double,freemem) ) );
	return true;
    }

    //Ensure all datapack arrays have contiguous memory
    const ObjectSet<TrcKeyZSampling>& stepstkzs =
				*volprocchainexecstepstkzsmgr_.getParam( this );
    for ( int idx=0; idx<stepstkzs.size(); idx++ )
    {
	const od_int64 nrsamples = stepstkzs[idx]->totalNr() * sizeof(float);
	if ( nrsamples < mChunkSize )
	    continue;

	nrexecs = mNINT32( Math::Ceil(	mCast(double,nrsamples) /
					mCast(double,mChunkSize) ) );
	return true;
    }

    nrexecs = 1;

    return false;
}

#undef mChunkSize


int VolProc::ChainExecutor::nrChunks( const TrcKeySampling&,
				      const StepInterval<int>&, int )
{ return 0; }


int VolProc::ChainExecutor::nrChunks( const TrcKeySampling& tks,
				      const StepInterval<float>& zsamp,
				      od_uint64& memusage )
{
    memusage = calculateMaximumMemoryUsage( tks, zsamp );
    const od_int64 nrbytes = mCast( od_int64, 1.01f * memusage );
    const bool cansplit = areSamplesIndependent() && !needsFullVolume();
    od_int64 freemem; int nrexecs;
    const bool needsplit = needSplit( nrbytes, freemem, nrexecs );
    if ( (needsplit && !cansplit) || freemem <=0 )
	return 0;
    else if ( !needsplit )
	return 1;

    int halfnrlinesperchunk = mNINT32( Math::Ceil( mCast(double,tks.nrLines()) /
						mCast(double,2 * nrexecs) ) );
    while ( halfnrlinesperchunk > 0 )
    {
	TrcKeySampling starthsamp( tks ), centerhsamp( tks ), stophsamp( tks );
	starthsamp.stop_.inl() = starthsamp.start_.inl() +2*halfnrlinesperchunk;
	centerhsamp.start_.inl() = tks.center().inl();
	centerhsamp.stop_.inl() = centerhsamp.start_.inl();
	centerhsamp.expand( halfnrlinesperchunk, 0 );
	stophsamp.start_.inl() = stophsamp.stop_.inl() - 2*halfnrlinesperchunk;
	starthsamp.limitTo( tks );
	centerhsamp.limitTo( tks );
	stophsamp.limitTo( tks );
	const od_uint64 memstart =
			calculateMaximumMemoryUsage( starthsamp, zsamp );
	const od_uint64 memcenter =
			calculateMaximumMemoryUsage( centerhsamp, zsamp );
	const od_uint64 memstop =
			calculateMaximumMemoryUsage( stophsamp, zsamp );
	memusage = memcenter;
	if ( memstart > memusage )
	    memusage = memstart;

	if ( memstop > memusage )
	    memusage = memstop;

	if ( mCast(od_uint64,1.01f*memusage) < freemem )
	    break;

	halfnrlinesperchunk--;
    }

    if ( halfnrlinesperchunk < 1 )
	return 0;

    return mNINT32( Math::Ceil( mCast(double,tks.nrLines() /
				mCast(double,2*halfnrlinesperchunk) ) ) );
}


int VolProc::ChainExecutor::getStepEpochIndex( const Step::ID stepid ) const
{ return -1; }


od_int64 VolProc::ChainExecutor::getStepOutputMemory( VolProc::Step::ID stepid,
			int nr, const TypeSet<TrcKeySampling>& epochstks,
			const TypeSet<StepInterval<int> >& epochszrg ) const
{ return -1; }


od_int64 VolProc::ChainExecutor::computeMaximumMemoryUsage(
			    const TrcKeySampling&, const StepInterval<int>& )
{ return 0; }


od_uint64 VolProc::ChainExecutor::calculateMaximumMemoryUsage(
					const TrcKeySampling& tks,
					const StepInterval<float>& zsamp )
{
    updateScheduledStepsSampling( tks, zsamp );
    adjustStepsNrComponents( tks.is2D() );
    const ObjectSet<TrcKeyZSampling>& stepstkzs =
				*volprocchainexecstepstkzsmgr_.getParam( this );

    TypeSet<VolumeMemory> activevolumes;
    for ( int epochidx=0; epochidx<epochs_.size(); epochidx++ )
    {
	const ObjectSet<Step>& steps = epochs_[epochidx]->getSteps();
	TypeSet<Step::ID> inputsids;
	for ( int stepidx=0; stepidx<steps.size(); stepidx++ )
	{
	    od_uint64 epochmemsize = 0;
	    const Step* step = steps[stepidx];
	    const int outputidx = 0;
	    //No actual support for multiple output datapacks yet!!
	    if ( !step->validOutputSlotID(step->getOutputSlotID(outputidx)))
		continue;

	    const int stepoutidx = chain_.indexOf( step );
	    const TrcKeyZSampling& outtkzs = *stepstkzs[stepoutidx];
	    epochmemsize += step->getComponentMemory( outtkzs, false ) *
			    step->getNrOutComponents();
	    epochmemsize += step->extraMemoryUsage( outputidx, outtkzs );

	    const int nrinputs = step->getNrInputs();
	    if ( nrinputs > 0 && !step->canInputAndOutputBeSame() )
	    {
		TypeSet<Chain::Connection> conns;
		web_.getConnections( step->getID(), true, conns );
		for ( int idx=0; idx<nrinputs; idx++ )
		{
		    const Step::ID inpstepid = conns[idx].outputstepid_;
		    if ( inputsids.isPresent(inpstepid) )
			continue;

		    inputsids += inpstepid;
		    const Step* inpstep = chain_.getStepFromID( inpstepid );
		    const int stepinpidx = chain_.indexOf( inpstep );
		    const TrcKeyZSampling& inptkzs = *stepstkzs[stepinpidx];
		    epochmemsize += inpstep->getComponentMemory(inptkzs,false) *
			  step->getNrInputComponents( conns[idx].inputslotid_ );
		}
	    }

	    VolumeMemory volmem(step->getID(),outputidx,epochmemsize,epochidx);
	    activevolumes += volmem;
	}
    }

    od_int64 res = 0;
    for ( int epochidx=0; epochidx<epochs_.size(); epochidx++ )
    {
	od_int64 memneeded = 0;

	for ( int idx=0; idx<activevolumes.size(); idx++ )
	{
	    if ( epochidx != activevolumes[idx].epoch_ )
		continue;

	    memneeded += activevolumes[idx].nrbytes_;
	}
	if ( memneeded > res )
	    res = memneeded;
    }

    return res;
}


bool VolProc::ChainExecutor::setCalculationScope( const TrcKeySampling&,
						  const StepInterval<int>&  )
{ return false; }


void VolProc::ChainExecutor::setOutputZSampling( const StepInterval<float>& )
{}


float VolProc::ChainExecutor::getSampleShift( float ) const
{ return mUdf(float); }


bool VolProc::ChainExecutor::areSamplesIndependent() const
{
    for ( int epochidx=0; epochidx<epochs_.size(); epochidx++ )
    {
	const ObjectSet<Step>& steps = epochs_[epochidx]->getSteps();
	for ( int stepidx=0; stepidx<steps.size(); stepidx++ )
	    if ( !steps[stepidx]->areSamplesIndependent() )
		return false;
    }
    return true;
}


bool VolProc::ChainExecutor::needsFullVolume() const
{
    for ( int epochidx=0; epochidx<epochs_.size(); epochidx++ )
    {
	const ObjectSet<Step>& steps = epochs_[epochidx]->getSteps();
	for ( int stepidx=0; stepidx<steps.size(); stepidx++ )
	    if ( steps[stepidx]->needsFullVolume() )
		return true;
    }
    return false;
}


VolProc::ChainExecutor::Epoch::Epoch( const ChainExecutor& ce )
    : taskgroup_(*new TaskGroup)
    , chainexec_(ce)
{
    taskgroup_.setName( ce.name() );
}


bool VolProc::ChainExecutor::Epoch::needsStepOutput( Step::ID stepid ) const
{
    for ( int idx=0; idx<steps_.size(); idx++ )
    {
	const Step* currentstep = steps_[idx];
	if ( currentstep->getID() == stepid )
	    return true;

	TypeSet<Chain::Connection> inputconnections;
	chainexec_.web_.getConnections( currentstep->getID(), true,
					inputconnections );
	for ( int idy=0; idy<inputconnections.size(); idy++ )
	    if ( inputconnections[idy].outputstepid_==stepid )
		return true;
    }

    return false;
}


bool VolProc::ChainExecutor::Epoch::updateInputs()
{
    for ( int idx=0; idx<steps_.size(); idx++ )
    {
	const Step* finishedstep = steps_[idx];
	const RegularSeisDataPack* input = finishedstep->getOutput();
	if ( !input )
	{
	    pErrMsg("Output is not available");
	    return false;
	}
	TypeSet<Chain::Connection> connections;
	chainexec_.web_.getConnections( finishedstep->getID(), false,
					connections );
	for ( int idy=0; idy<connections.size(); idy++ )
	{
	    Step* futurestep = chainexec_.chain_.getStepFromID(
				     connections[idy].inputstepid_ );
	    if ( !futurestep )
		continue;

	    const Step::OutputSlotID finishedslot =
					connections[idy].outputslotid_;
	    const int outputidx = finishedstep->getOutputIdx( finishedslot );
	    if ( !input->validComp(outputidx) )
	    {
		pErrMsg("Output is not available");
		return false;
	    }

	    futurestep->setInput( connections[idy].inputslotid_, input );
	}
    }

    return true;
}


bool VolProc::ChainExecutor::Epoch::doPrepare( ProgressMeter* progmeter )
{
    for ( int idx=0; idx<steps_.size(); idx++ )
    {
	Step* currentstep = steps_[idx];
	PosInfo::CubeData trcssampling;
	for ( int idy=0; idy<currentstep->getNrInputs(); idy++ )
	{
	    const Step::InputSlotID inputslot =
				    currentstep->getInputSlotID( idy );
	    if ( !currentstep->validInputSlotID(inputslot) )
	    {
		pErrMsg("This should not happen");
		return false;
	    }

	    const RegularSeisDataPack* input = currentstep->getInput(inputslot);
	    if ( input && input->getTrcsSampling() )
		trcssampling.merge( *input->getTrcsSampling(), true );
	}

	TrcKeyZSampling csamp;
	if ( !chainexec_.getCalculationScope(currentstep->getID(),csamp) ||
	     !csamp.isDefined() )
	{
	    BufferString msg( "This should not happen" );
	    errmsg_ = msg;
	    pErrMsg(msg);
	    return false;
	}

	const RegularSeisDataPack* outfrominp =
			currentstep->canInputAndOutputBeSame() &&
			currentstep->validInputSlotID(0) ?
		currentstep->getInput( currentstep->getInputSlotID(0) ) : 0;

	RegularSeisDataPack* outcube = outfrominp
			     ? const_cast<RegularSeisDataPack*>( outfrominp )
			     : new RegularSeisDataPack( 0 );
	if ( outfrominp )
	{
	    if ( !outcube ||
		 !DPM( DataPackMgr::SeisID() ).obtain(outcube->id()) )
	    {
		if ( outcube ) outcube->release();
		return false;
	    }
	}
	else
	{
	    outcube->setName( "New VolProc DP" );
	    DPM( DataPackMgr::SeisID() ).addAndObtain( outcube );
	    outcube->setSampling( csamp );
	    if ( trcssampling.totalSizeInside( csamp.hsamp_ ) > 0 )
	    {
		trcssampling.limitTo( csamp.hsamp_ );
		if ( !trcssampling.isFullyRectAndReg() )
		{
		    outcube->setTrcsSampling(
			    new PosInfo::SortedCubeData(trcssampling));
		}
	    }

	    for ( int icomp=0; icomp<currentstep->getNrOutComponents(); icomp++)
	    {
		if ( !outcube->addComponentNoInit(0) )
		{
		    errmsg_ = "Cannot allocate enough memory.";
		    outcube->release();
		    return false;
		}
	    }
	}

	Step::OutputSlotID outputslotid = 0; // TODO: get correct slotid
	TrcKeySampling unusedtks; StepInterval<int> unusedzrg;
	currentstep->setOutput( outputslotid, outcube, unusedtks, unusedzrg );

	//The step should have reffed it, so we can forget it now.
	outcube->release();

	TypeSet<Chain::Connection> outputconnections;
	chainexec_.web_.getConnections( currentstep->getID(),
					false, outputconnections );

	for ( int idy=0; idy<outputconnections.size(); idy++ )
	     currentstep->enableOutput( outputconnections[idy].outputslotid_ );

	if ( currentstep->getID()==chainexec_.chain_.outputstepid_ )
	    currentstep->enableOutput( chainexec_.chain_.outputslotid_ );

	Task* newtask = currentstep->needReportProgress()
			    ? currentstep->createTaskWithProgMeter(progmeter)
			    : currentstep->createTask();
	if ( !newtask )
	{
	    pErrMsg("Could not create task");
	    return false;
	}

	taskgroup_.addTask( newtask );
    }

    return true;
}


void VolProc::ChainExecutor::Epoch::releaseData()
{
    for ( int idx=0; idx<steps_.size(); idx++ )
	steps_[idx]->releaseData();
}


const RegularSeisDataPack* VolProc::ChainExecutor::Epoch::getOutput() const
{
    return steps_[steps_.size()-1] ? steps_[steps_.size()-1]->getOutput() : 0;
}


const RegularSeisDataPack* VolProc::ChainExecutor::getOutput() const
{
    return outputdp_;
}


#define mCleanUpAndRet( prepare ) \
{ \
    uiStringSet errors; \
    const ObjectSet<Step>& cursteps = curepoch_->getSteps(); \
    for ( int istep=0; istep<cursteps.size(); istep++ ) \
    { \
	if ( !cursteps[istep] || cursteps[istep]->errMsg().isEmpty() ) \
	    continue; \
\
	errors.add( cursteps[istep]->errMsg() ); \
    } \
    if ( prepare && !curepoch_->errMsg().isEmpty() ) \
	errors.add( toUiString(curepoch_->errMsg()) ); \
    else if ( !prepare && curepoch_->getTask().uiMessage().isSet() ) \
	errors.add( curepoch_->getTask().uiMessage() ); \
    \
    if ( !errors.isEmpty() ) \
	errmsg_ = errors.cat(); \
    deleteAndZeroPtr( curepoch_ ); \
    return ErrorOccurred(); \
}

int VolProc::ChainExecutor::nextStep()
{
    if ( !isok_ )
	return ErrorOccurred();
    if ( epochs_.isEmpty() )
	return Finished();

    curepoch_ = epochs_.pop();

    if ( !curepoch_->doPrepare(progressmeter_) )
	mCleanUpAndRet( true )

    Task& curtask = curepoch_->getTask();
    curtask.setProgressMeter( progressmeter_ );
    curtask.enableWorkControl( true );
    if ( !curtask.execute() )
	mCleanUpAndRet( false )

    const bool finished = epochs_.isEmpty();
    if ( finished )		//we just executed the last one
    {
	outputdp_ = curepoch_->getOutput();
	if ( !outputdp_ ||
	     !DPM( DataPackMgr::SeisID() ).obtain(outputdp_->id()) )
	{
	    if ( outputdp_ )
	    {
		const_cast<RegularSeisDataPack*>( outputdp_ )->release();
		outputdp_ = 0;
	    }

	    return false;
	}
    }

    //Give output volumes to all steps that need them
    if ( !finished && !curepoch_->updateInputs() )
	return false;

    //Everyone who wants my data has it. I can release it.
    curepoch_->releaseData();
    deleteAndZeroPtr( curepoch_ );
    if ( finished )
	progressmeter_ = 0;

    return finished ? Finished() : MoreToDo();
}


void VolProc::ChainExecutor::releaseMemory()
{
    ObjectSet<Step> stepstorelease = scheduledsteps_;
    for ( int idx=stepstorelease.size()-1; idx>=0; idx-- )
    {
	const Step::ID curid = stepstorelease[idx]->getID();
	for ( int idy=0; idy<epochs_.size(); idy++ )
	{
	    if ( epochs_[idy]->needsStepOutput( curid ) )
	    {
		stepstorelease.removeSingle( idx );
		break;
	    }
	}
    }

    for ( int idx=stepstorelease.size()-1; idx>=0; idx-- )
	stepstorelease[idx]->releaseData();
}


void VolProc::ChainExecutor::controlWork( Task::Control ctrl )
{
    Task::controlWork( ctrl );
    if ( curepoch_ )
	curepoch_->getTask().controlWork( ctrl );
}


void VolProc::ChainExecutor::setJobCommunicator( JobCommunic* jc )
{ jobcomm_ = jc; }


od_int64 VolProc::ChainExecutor::nrDone() const
{
    if ( totalnrepochs_ < 1 )
	return 0;

    const float percentperepoch = 100.f / totalnrepochs_;
    const int epochsdone = totalnrepochs_ - epochs_.size() - 1;
    float percentagedone = percentperepoch * epochsdone;

    if ( curepoch_ )
    {
	Task& curtask = curepoch_->getTask();

	const od_int64 nrdone = curtask.nrDone();
	const od_int64 totalnr = curtask.totalNr();
	if ( nrdone>=0 && totalnr>0 )
	{
	    const float curtaskpercentage = percentperepoch * nrdone / totalnr;
	    percentagedone += curtaskpercentage;
	}
    }

    if ( jobcomm_ )
	jobcomm_->updateProgress( mNINT32(percentagedone) );

    return mNINT64( percentagedone );
}


uiString VolProc::ChainExecutor::uiNrDoneText() const
{
    return uiStrings::sPercentageDone();
}


od_int64 VolProc::ChainExecutor::totalNr() const
{
    return 100;
}


uiString VolProc::ChainExecutor::uiMessage() const
{
    if ( !errmsg_.isEmpty() )
	return errmsg_;

    if ( curepoch_ )
	return curepoch_->getTask().uiMessage();

    return uiString::emptyString();
}
