/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : A.H. Bril
 * DATE     : 3-8-1994
-*/

static const char* rcsID mUsedVar = "$Id$";

#include "dirlist.h"

#include "file.h"
#include "filepath.h"
#include "perthreadrepos.h"

#ifndef OD_NO_QT
# include <QDir>
#endif

DirList::DirList( const char* dirname, DirList::Type t, const char* msk )
	: dir_(dirname?dirname:".")
	, type_(t)
	, mask_(msk)
{
    update();
}


void DirList::update()
{
#ifndef OD_NO_QT
    erase();
    const bool havemask = !mask_.isEmpty();

    QDir qdir( dir_.buf() );
    if ( havemask )
    {
	QStringList filters;
	filters << mask_.buf();
	qdir.setNameFilters( filters );
    }

    QDir::Filters filters;
    if ( type_ == FilesOnly )
	filters = QDir::Files | QDir::Hidden;
    else if ( type_ == DirsOnly )
	filters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden;
    else
	filters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Files
			     | QDir::Hidden;

    QStringList qlist = qdir.entryList( filters );
    for ( int idx=0; idx<qlist.size(); idx++ )
	add( qlist[idx] );

    sort();
#endif
}


const char* DirList::fullPath( int idx ) const
{
    mDeclStaticString( ret );
    if ( idx < size() )
	ret = FilePath( dirName(), (*this).get(idx) ).fullPath();
    else
	ret.setEmpty();
    return ret.buf();
}
