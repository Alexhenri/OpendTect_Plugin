/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        Bert
 Date:          Oct 2010
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "moddepmgr.h"
#include "uistratbasiclayseqgendesc.h"

mDefModInitFn(uiStrat)
{
    mIfNotFirstTime( return );

    uiBasicLayerSequenceGenDesc::initClass();
}
