#ifndef uitutmadagascarmod_h
#define uitutmadagascarmod_h

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

#if defined(uiTutMadagascar_EXPORTS) || defined(UITUTMADAGASCAR_EXPORTS)
# define do_export_uiTutMadagascar
#else
# if defined ( do_export_uiTutMadagascar )
#  undef do_export_uiTutMadagascar
# endif
#endif


#if defined( do_export_uiTutMadagascar )
# define Export_uiTutMadagascar	dll_export
# define Extern_uiTutMadagascar
#else
# define Export_uiTutMadagascar	dll_import
# define Extern_uiTutMadagascar	dll_extern
#endif

#if defined ( do_import_export )



#endif

#endif
