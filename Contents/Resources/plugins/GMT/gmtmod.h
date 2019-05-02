#ifndef gmtmod_h
#define gmtmod_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	K. Tingdahl
 Date:		Mar 2006
 RCS:		$Id$
________________________________________________________________________


-*/

//
//This file is generated automatically from CMAKE. It contains export/import
//Declarations that are used on windows. It also includes deps of all modules
//that this module is dependent on.
//

#if defined( __win64__ ) || defined ( __win32__ )
# define do_import_export
#else
# ifdef do_import_export
#  undef do_import_export
# endif
#endif

#ifndef dll_export
# if defined( do_import_export )
#  define dll_export	__declspec( dllexport )
#  define dll_import	__declspec( dllimport )
#  define dll_extern	extern
# else
#  define dll_export
#  define dll_import
# endif
#endif

#if defined(GMT_EXPORTS) || defined(GMT_EXPORTS)
# define do_export_GMT
#else
# if defined ( do_export_GMT )
#  undef do_export_GMT
# endif
#endif


#if defined( do_export_GMT )
# define Export_GMT	dll_export
# define Extern_GMT
#else
# define Export_GMT	dll_import
# define Extern_GMT	dll_extern
#endif

#if defined ( do_import_export )



#endif

#endif
