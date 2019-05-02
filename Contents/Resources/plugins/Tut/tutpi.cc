
/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : A.H. Bril
 * DATE     : May 2007
-*/

static const char* rcsID mUsedVar = "$Id$";

#include "tutseistools.h"
#include "tutorialattrib.h"
#include "tutvolproc.h"
#include "odplugin.h"


mDefODPluginEarlyLoad(Tut)
mDefODPluginInfo(Tut)
{
    mDefineStaticLocalObject( PluginInfo, retpi,(
	"Tutorial plugin Base",
	"OpendTect",
	"dGB (Raman/Bert)",
	"3.2",
    	"Back-end for the plugin that shows simple plugin development basics."
    	"\nThis non-UI part can also be loaded into od_process_attrib." ) );
    return &retpi;
}


mDefODInitPlugin(Tut)
{
    Attrib::Tutorial::initClass();
    VolProc::TutOpCalculator::initClass();

    return 0;
}
