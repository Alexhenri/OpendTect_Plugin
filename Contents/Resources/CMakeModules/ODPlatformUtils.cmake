#_______________________Pmake__________________________________________________
#
#	CopyRight:	dGB Beheer B.V.
# 	Jan 2012	K. Tingdahl
#	RCS :		$Id$
#_______________________________________________________________________________

#Discover 64 or 32 bits
set ( OD_64BIT 1 )
if ( CMAKE_SIZEOF_VOID_P EQUAL 4 )
    set ( OD_64BIT )
endif()

#Discover Debug
if( ${CMAKE_BUILD_TYPE} MATCHES Release)
    set ( OD_DEBUG )
else()
    set ( OD_DEBUG 1 )
endif()

set ( SET_SYMBOLS -D__hassymbols__ )
set ( SET_DEBUG -D__debug__ )

set ( CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${SET_SYMBOLS} ")
set ( CMAKE_C_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${SET_SYMBOLS} ")

if(UNIX) #Apple an Linux
    if ( CMAKE_COMPILER_IS_GNUCC  )
	set ( OD_GCC_COMPILER 1 )
	execute_process( COMMAND ${CMAKE_C_COMPILER} -dumpversion
			 OUTPUT_VARIABLE GCC_VERSION )

    endif(CMAKE_COMPILER_IS_GNUCC)
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

    if(APPLE)
	option( AVOID_CLANG_ERROR "Avoid CLang error" OFF )
	if ( AVOID_CLANG_ERROR )
	    set ( CMAKE_CXX_FLAGS
		  "${CMAKE_CXX_FLAGS} -D__MAC_LLVM_COMPILER_ERROR__" )
	endif()
	set( CMAKE_MACOSX_RPATH 1 )
	set( CMAKE_INSTALL_RPATH "@loader_path/../Frameworks" )
	set ( OD_GCC_COMPILER 1 )
	if ( ${CMAKE_GENERATOR} STREQUAL "Xcode" )
	    set ( OD_EXTRA_OSGFLAGS "-Wno-shadow -Wno-overloaded-virtual" ) #Sysroot does not do the job
	    set ( OD_EXTRA_COINFLAGS "-Wno-shadow -Wno-overloaded-virtual" ) #Sysroot does not do the job
	endif()

	#For some versions of XCode
	set ( CMAKE_FIND_LIBRARY_PREFIXES lib )
	set ( CMAKE_FIND_LIBRARY_SUFFIXES .dylib )

	set ( OD_LIB_LINKER_NEEDS_ALL_LIBS 1 )
	set ( OD_PLATFORM_LINK_OPTIONS "-arch x86_64" )
        add_definitions("-arch x86_64")
        find_library( APP_SERVICES_LIBRARY ApplicationServices
		      PATH ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks )
        find_library( STDCPP_LIBRARY stdc++ REQUIRED )
        set (EXTRA_LIBS ${APP_SERVICES_LIBRARY} )
	set (OD_SUPPRESS_UNDEF_FLAGS "-flat_namespace -undefined suppress" )
	#set ( OD_GUI_SYSTEM "MACOSX_BUNDLE" )

	set ( OD_PLFSUBDIR mac )

	if ( (GCC_VERSION VERSION_GREATER 4.2.1) OR (NOT DEFINED CMAKE_COMPILER_IS_GNUCC) )
	    set ( CMAKE_CXX_FLAGS "-Wdelete-non-virtual-dtor ${CMAKE_CXX_FLAGS}" )
	endif()

    else() #Linux
	#Not on most platforms, but for the few that does, it's better
	set (OD_LIB_LINKER_NEEDS_ALL_LIBS 1)

	if ( OD_64BIT )
	    set ( OD_PLFSUBDIR "lux64" )
	    set ( OD_EXECUTABLE_COMPILE_FLAGS "-fPIC" )
	else()
	    set ( OD_PLFSUBDIR "lux32" )
	    add_definitions("-march=pentium4")
	endif()

	if ( CMAKE_COMPILER_IS_GNUCC  )
	    if ( GCC_VERSION VERSION_GREATER 4.2 )
		set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wignored-qualifiers" )
	    endif()

	    set ( CMAKE_CXX_FLAGS "-Wno-non-template-friend ${CMAKE_CXX_FLAGS}" )
	endif(CMAKE_COMPILER_IS_GNUCC)

    endif()

    set ( OD_LINESEP "\n" )
    set ( CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem ")
    set ( CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")
    add_definitions("'-DmUnusedVar=__attribute__ ((unused))'")
    add_definitions("'-DmUsedVar=__attribute__ ((used))'")
    set (OD_STATIC_EXTENSION ".a")
    if ( OD_GCC_COMPILER )
	set ( CMAKE_CXX_FLAGS "-Woverloaded-virtual -Wno-reorder ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wunused -Wmissing-braces -Wparentheses -Wsequence-point ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wswitch -Wunused-function -Wunused-label ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wshadow -Wwrite-strings -Wpointer-arith -Winline ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wformat -Wmissing-field-initializers ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wreturn-type -Winit-self -Wno-char-subscripts ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wstrict-aliasing ${CMAKE_CXX_FLAGS}" )

	#use below and you'll be flooded with warnings:
	#set ( CMAKE_CXX_FLAGS "-Wno-sign-compare -Wcast-align -Wconversion ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS "-Wno-sign-compare -Wcast-align ${CMAKE_CXX_FLAGS}" )


	set ( CMAKE_CXX_FLAGS_RELEASE "-Wno-inline -Wuninitialized -Winit-self ${CMAKE_CXX_FLAGS_RELEASE}" )
	if ( UNIX )
	    if( "${OD_ENABLE_BREAKPAD}" STREQUAL "ON" )
		if ( DEFINED BREAKPAD_DIR AND EXISTS ${BREAKPAD_DIR} )
		    set ( CMAKE_CXX_FLAGS_RELEASE "-g ${CMAKE_CXX_FLAGS_RELEASE}" )
		endif()
	    endif()
	endif()

	set ( CMAKE_C_FLAGS "-Wmissing-declarations -Wunused -Wimplicit-int ${CMAKE_C_FLAGS}" )
	set ( CMAKE_C_FLAGS "-Wimplicit-function-declaration -Wpointer-sign ${CMAKE_C_FLAGS}" )
	set ( CMAKE_C_FLAGS "-Wstrict-aliasing -Wstrict-prototypes ${CMAKE_C_FLAGS}" )

	set ( CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} ${SET_SYMBOLS} ${SET_DEBUG} -ggdb3" )
	set ( CMAKE_C_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} ${SET_SYMBOLS} ${SET_DEBUG} -ggdb3" )

    else() # Intel compiler
	set ( CMAKE_SKIP_RPATH TRUE )
	set (EXTRA_LIBS "imf" "m") #avoid bogus warning: https://wiki.hpcc.msu.edu/display/Issues/feupdateenv+is+not+implemented+and+will+always+fail
    endif( OD_GCC_COMPILER )

endif(UNIX)

if(WIN32)
    #Create Launchers on Windows
    set ( OD_CREATE_LAUNCHERS 1 )
    set ( OD_SET_TARGET_PROPERTIES 1 )

    set ( OD_LIB_LINKER_NEEDS_ALL_LIBS 1)
    set ( OD_PLATFORM_LINK_OPTIONS "/LARGEADDRESSAWARE" )

    #Setting Stack Reserve size for Executables only
    if ( NOT DEFINED STACK_RESERVE_SIZE )
	MATH ( EXPR STACK_RESERVE_SIZE "8 * 1024 * 1024" ) #Setting default stack size to 8MB
	set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:${STACK_RESERVE_SIZE}" )
    else()
	set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:${STACK_RESERVE_SIZE}" )
    endif()
    set( STACK_RESERVE_SIZE ${STACK_RESERVE_SIZE} CACHE STRING "Stack Reserve Size" )

    if ( CTEST_MODEL STREQUAL "Experimental" )
	set ( OD_PLATFORM_LINK_OPTIONS "${OD_PLATFORM_LINK_OPTIONS} /debug" )
	#/debug will enable the generation of pdb-files.
	set ( CMAKE_CXX_FLAGS " /Z7 ${CMAKE_CXX_FLAGS}" ) #/Zi for additional debug info to the .pdb file. 
    endif()

    set ( CMAKE_CXX_FLAGS "/vmg /EHsc ${CMAKE_CXX_FLAGS}")
    #set ( CMAKE_CXX_FLAGS "/MP")
    set (EXTRA_LIBS "ws2_32" "shlwapi")
    set ( CMAKE_CXX_FLAGS   "-DmUnusedVar= ${CMAKE_CXX_FLAGS}")
    set ( CMAKE_CXX_FLAGS   "-DmUsedVar= ${CMAKE_CXX_FLAGS}")
    set ( CMAKE_C_FLAGS   "-DmUnusedVar= ${CMAKE_C_FLAGS}")
    set ( CMAKE_C_FLAGS   "-DmUsedVar= ${CMAKE_C_FLAGS}")
    string ( REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
    set ( CMAKE_CXX_FLAGS " /W4 ${CMAKE_CXX_FLAGS}" )
    if ( DEFINED ${QTDIR} )
        if ( NOT DEFINED Qt5Core_DIR OR  Qt5Core_DIR STREQUAL "" )
	    message( "Info: Using Qt4 that does NOT treat wchar as built in type" )
	    set ( CMAKE_CXX_FLAGS "/Zc:wchar_t- ${CMAKE_CXX_FLAGS}" )
        endif()
    endif()

    set ( CMAKE_CXX_FLAGS  "/wd4389 ${CMAKE_CXX_FLAGS}" ) # unsigned/signed mismatch
    set ( CMAKE_CXX_FLAGS  "/wd4458 ${CMAKE_CXX_FLAGS}" ) # warnings for Microsoft's GDI+ headers
    set ( CMAKE_CXX_FLAGS  "/wd4018 ${CMAKE_CXX_FLAGS}" ) # unsigned/signed compare
    set ( CMAKE_CXX_FLAGS  "/wd4505 ${CMAKE_CXX_FLAGS}" ) # unreferenced local function removed
    set ( CMAKE_CXX_FLAGS  "/wd4121 ${CMAKE_CXX_FLAGS}" ) # Alignmnent of variables
    set ( CMAKE_CXX_FLAGS  "/wd4250 ${CMAKE_CXX_FLAGS}" ) # Diamond inheritance problems
    set ( CMAKE_CXX_FLAGS  "/wd4355 ${CMAKE_CXX_FLAGS}" ) # The this pointer is valid only within nonstatic member functions.
    set ( CMAKE_CXX_FLAGS  "/wd4100 ${CMAKE_CXX_FLAGS}" ) # unreferenced formal parameter
    #set ( CMAKE_CXX_FLAGS " /wd4701 ${CMAKE_CXX_FLAGS}" ) # local variable used without being initialized
    set ( CMAKE_CXX_FLAGS  "/wd4800 ${CMAKE_CXX_FLAGS}" ) # forcing value to bool 'true' or 'false' (performance warning)
    set ( CMAKE_CXX_FLAGS  "/wd4251 ${CMAKE_CXX_FLAGS}" ) # 'identifier' : dll-interface
    set ( CMAKE_CXX_FLAGS  "/wd4275 ${CMAKE_CXX_FLAGS}" ) # 'identifier' : dll-interface
    set ( CMAKE_CXX_FLAGS  "/wd4273 ${CMAKE_CXX_FLAGS}" ) # inconsistent dll linkage
    set ( CMAKE_CXX_FLAGS  "/wd4996 ${CMAKE_CXX_FLAGS}" ) # function': was declared deprecated
    set ( CMAKE_CXX_FLAGS  "/wd4101 ${CMAKE_CXX_FLAGS}" ) # The local variable is never used (disable only for Windows)
    set ( CMAKE_CXX_FLAGS  "/wd4267 ${CMAKE_CXX_FLAGS}" ) # conversion from 'size_t' to 'type', possible loss of data
    set ( CMAKE_CXX_FLAGS  "/wd4267 ${CMAKE_CXX_FLAGS}" ) # conversion from 'size_t' to 'type', possible loss of data
    set ( CMAKE_CXX_FLAGS  "/wd4512 ${CMAKE_CXX_FLAGS}" ) # class' : assignment operator could not be generated (not important)
    set ( CMAKE_CXX_FLAGS  "/wd4127 ${CMAKE_CXX_FLAGS}" ) # conditional expression is constant, e.g. while ( true )
    set ( CMAKE_CXX_FLAGS  "/wd4189 ${CMAKE_CXX_FLAGS}" ) # local variable is initialized but not referenced
    set ( CMAKE_CXX_FLAGS  "/wd4305 ${CMAKE_CXX_FLAGS}" ) # truncation from dowble to float
    if ( NOT MSVC14 )
	set ( CMAKE_CXX_FLAGS_DEBUG  "/WX ${CMAKE_CXX_FLAGS_DEBUG}" ) # Treat warnings as errors
    else()
	set ( CMAKE_CXX_FLAGS  "/wd4311 ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS  "/wd4302 ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS  "/wd4714 ${CMAKE_CXX_FLAGS}" )
	set ( CMAKE_CXX_FLAGS  "/wd4838 ${CMAKE_CXX_FLAGS}" )
    endif()

    string ( REPLACE  "/Zi" "/Z7" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE} )
    string ( REPLACE  "/Zi" "/Z7" CMAKE_C_FLAGS_MINSIZEREL ${CMAKE_C_FLAGS_MINSIZEREL} )

    set (OD_STATIC_EXTENSION ".lib")
    set (OD_EXECUTABLE_EXTENSION ".exe" )
    if ( OD_64BIT )
        set ( OD_PLFSUBDIR "win64" )
    else()
        set ( OD_PLFSUBDIR "win32" )
	set ( CMAKE_CXX_FLAGS "/wd4244 ${CMAKE_CXX_FLAGS}" ) # conversion' conversion from 'type1' to 'type2', possible loss of data ( _int64 to int ) 
    endif()
    if ( MSVC14 )
	set ( CMAKE_CXX_FLAGS "/wd4244 ${CMAKE_CXX_FLAGS}" )
    endif()

    set ( OD_GUI_SYSTEM "WIN32" )
    set ( OD_LINESEP "\n" ) #Will be converted to \r\n when written to files by cmake

    if ( CMAKE_GENERATOR STREQUAL "Ninja" )
        set ( OD_UAC_LINKFLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"" )
    else()
        set ( OD_UAC_LINKFLAGS "/level='requireAdministrator' /uiAccess='false'" )
    endif()

endif()

if ( WIN32 )
    add_definitions( "-D__${OD_PLFSUBDIR}__=1" )
else()
    add_definitions( "\"-D__${OD_PLFSUBDIR}__=1\"" )
endif()