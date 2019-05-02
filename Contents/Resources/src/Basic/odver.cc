/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:        Bert
 Date:          Mar 2007
 RCS:           $Id$
________________________________________________________________________

-*/

static const char* rcsID mUsedVar = "$Id$";

#include "odver.h"
#include "oddirs.h"
#include "odinst.h"
#include "odplatform.h"
#include "genc.h"
#include "file.h"
#include "filepath.h"
#include "keystrs.h"
#include "perthreadrepos.h"
#ifndef OD_NO_QT
#include "qglobal.h"
#endif
#include <string.h>


mDefineEnumUtils(OD::Platform,Type,"Platform")
{
	"Linux (32 bits)",
	"Linux (64 bits)",
	"Windows (32 bits)",
	"Windows (64 bits)",
	"Mac OS X",
	0
};


extern "C" const char* GetFullODVersion()
{
    mDeclStaticString( ret );
    if ( !ret.isEmpty() ) return ret.buf();

    GetSpecificODVersion( 0, ret );

    if ( ret.isEmpty() )
    {
	const char* pvnm = GetProjectVersionName();
	pvnm = lastOcc( pvnm, 'V' );
	if ( pvnm )
	    ret = pvnm + 1;

	if ( ret.isEmpty() )
	    ret = "0.0.0";
    }

    return ret.buf();
}


void GetSpecificODVersion( const char* typ, BufferString& res )
{
    res = ODInst::getPkgVersion( typ ? typ : "basedata" );
    if ( res.matches( "*error*" ) )
	res.setEmpty();
}


const char* GetGCCVersion()
{
#ifndef __GNUC__
    return sKey::EmptyString();
#else
    mDeclStaticString( ret );
    if ( !ret.isEmpty() ) return ret.buf();

    ret.set( __GNUC__ ).add( "." )
       .add( __GNUC_MINOR__ ).add( "." )
       .add( __GNUC_PATCHLEVEL__ );
    return ret.buf();
#endif
}


const char* GetMSVCVersion()
{
#ifndef __msvc__
    return sKey::EmptyString();
#else
    mDeclStaticString( ret );
    if ( !ret.isEmpty() ) return ret.buf();
    ret.set( _MSC_VER );
    return ret;
#endif
}


const char* GetMSVCVersionStr()
{
#ifndef __msvc__
    return sKey::EmptyString();
#else
    mDeclStaticString( ret );
    if ( !ret.isEmpty() ) return ret.buf();

# if ( _MSC_VER == 1800 )
    ret = "Visual Studio 2013 - MSVC 12.0";
# elif ( _MSC_VER == 1700 )
    ret = "Visual Studio 2012 - MSVC 11.0";
# elif ( _MSC_VER == 1600 )
    ret = "Visual Studio 2010 - MSVC 10.0";
# elif ( _MSC_VER == 1500 )
    ret = "Visual Studio 2008 - MSVC 9.0";
# elif ( _MSC_VER == 1400 )
    ret = "Visual Studio 2005 - MSVC 8.0";
# endif
    return ret;
#endif
}


const char* GetCompilerVersionStr()
{
    mDeclStaticString( ret );
#ifdef __win__
    ret = GetMSVCVersionStr();
#else
    ret.set( "GCC " ).add( GetGCCVersion() );
#endif
    return ret;
}


const char* GetQtVersion()
{
    mDeclStaticString( ret );
#ifndef OD_NO_QT
    if ( !ret.isEmpty() ) return ret.buf();

    ret.set( QT_VERSION_STR );
#endif
    return ret.buf();
}


const OD::Platform& OD::Platform::local()
{
    mDefineStaticLocalObject( Platform, theplf, );
    return theplf;
}


OD::Platform::Platform()
    : type_(__ismac__ ?				     Mac
		      : (__iswin__ ? (__is32bits__ ? Win32 : Win64)
				   : (__is32bits__ ? Lin32 : Lin64)))
{
}


const char* OD::Platform::shortName() const
{
    return isLinux()	? (type_ == Lin32 ? "lux32" : "lux64")
	: (isWindows()	? (type_ == Win32 ? "win32" : "win64")
					  : "mac" );
}


void OD::Platform::set( const char* s, bool isshort )
{
    if ( !s || !*s )
	{ pErrMsg("null or empty platform set"); return; }

    if ( !isshort )
	parseEnumType( s, type_ );
    else
    {
	const bool islin = *s == 'l';
	const bool iswin = *s == 'w';
	const bool is64 = *(s+strlen(s)-1) == '4';

	type_ = islin ? (is64 ? Lin64 : Lin32)
	    : (iswin ? (is64 ? Win64 : Win32)
		     : Mac);
    }
}


bool OD::Platform::isValidName( const char* s, bool isshort )
{
    if ( !s || !*s )	return false;
    if ( !isshort )	return TypeDef().isValidName( s );

    const BufferString cmp(s);
    return cmp == "lux64" || cmp == "win64"
	|| cmp == "lux32" || cmp == "win32"
	|| cmp == "mac";
}


static BufferStringSet legalinfo_;

const BufferStringSet&	 GetLegalInformation()
{ return legalinfo_; }


void AddLegalInformation( const char* txt )
{
    legalinfo_.add( txt );
}