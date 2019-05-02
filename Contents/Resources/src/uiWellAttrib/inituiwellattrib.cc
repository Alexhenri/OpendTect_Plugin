/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	Bert
 Date:		Nov 2010
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";

#include "moddepmgr.h"
#include "uistratlayermodel.h"
#include "uiwelllogattrib.h"

mDefModInitFn(uiWellAttrib)
{
    mIfNotFirstTime( return );

    uiStratLayerModel::initClass();
    uiWellLogAttrib::initClass();
}
