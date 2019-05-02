#_______________________Pmake__________________________________________________
#
#	CopyRight:	dGB Beheer B.V.
# 	Jan 2012	K. Tingdahl
#	RCS :		$Id$
#_______________________________________________________________________________

MACRO( OD_SETUP_ZLIB )
    if ( NOT DEFINED APPLE ) #QT never includes zlib on Mac
	if ( Qt5Core_FOUND )
	    if ( EXISTS "${QTDIR}/include/QtZlib/zlib.h" )
		set( ZLIB_INCLUDE_DIR ${QTDIR}/include/QtZlib )
		set ( ZLIB_LIBRARY ${Qt5Core_LIBRARIES} )
	    endif()
	elseif ( QT_QTCORE_INCLUDE_DIR )
	    if ( EXISTS "${QT_QTCORE_INCLUDE_DIR}/zlib.h" )
		set ( ZLIB_INCLUDE_DIR ${QT_QTCORE_INCLUDE_DIR} )
		set ( ZLIB_LIBRARY ${QT_QTCORE_LIBRARY} )
	    endif()
	endif()
    endif()

    if ( NOT DEFINED ZLIB_INCLUDE_DIR )
	find_package( Zlib REQUIRED )
    endif()

    if ( DEFINED ZLIB_INCLUDE_DIR )
	add_definitions( -DHAS_ZLIB )
    endif()
ENDMACRO()
