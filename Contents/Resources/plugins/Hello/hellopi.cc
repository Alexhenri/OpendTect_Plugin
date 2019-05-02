/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : A.H. Bril
 * DATE     : Sep 2003 / Apr 2011
-*/

static const char* rcsID mUsedVar = "$Id$";

#include "hellomod.h"

#include "odplugin.h"
#include "od_ostream.h"

mDefODInitPlugin(Hello)
{
    od_cout() << "Hello world" << od_endl;
    return 0; // All OK - no error messages
}
