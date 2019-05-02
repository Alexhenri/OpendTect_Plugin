/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        A.H.Bril
 Date:          Jul 2006
________________________________________________________________________

-*/
static const char* rcsID mUsedVar = "$Id$";


#include "prog.h"
#include "genc.h"
#include <iostream>

extern int ODSysAdmMain(int,char**);

int main( int argc, char** argv )
{
    SetProgramArgs( argc, argv );

    int ret = ODSysAdmMain( argc, argv );
    return ExitProgram( ret );
}
