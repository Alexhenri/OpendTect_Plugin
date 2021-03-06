/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : K. Tingdahl
 * DATE     : Nov 2015
 * FUNCTION : Clipboard functionality
-*/

static const char* rcsID mUsedVar = "$Id$";

#include "uiclipboard.h"

#include "uistring.h"

#include <QApplication>
#include <QClipboard>

void uiClipboard::setText( const uiString& str )
{
     QClipboard* clipboard = QApplication::clipboard();
     clipboard->clear();
     QString qstr;
     str.fillQString( qstr );
     clipboard->setText( qstr );
}


void uiClipboard::setImage( const QImage& img )
{
     QClipboard* clipboard = QApplication::clipboard();
     clipboard->clear();
     clipboard->setImage( img );
}
