/*+
 * (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 * AUTHOR   : K. Tingdahl
 * DATE     : Dec 2014
-*/

static const char* rcsID mUsedVar = "$Id$";

#ifndef OD_NO_QT

#include "qstreambuf.h"
#include "ptrman.h"
# include <QProcess>

#define mPUSH_BACK	100
#define mBUFFER_SIZE	1000

#define mTIMEOUT	100


qstreambuf::qstreambuf( QIODevice& p, bool isstderr, bool own )
    : iodevice_( &p )
    , isstderr_( isstderr )
    , buffer_( mPUSH_BACK+mBUFFER_SIZE )
    , ownsdevice_( own )
{
    mDynamicCast( QProcess*, process_, iodevice_ );

    char* end = &buffer_.front()+buffer_.size();
    setg( end, end, end );
}


qstreambuf::~qstreambuf()
{
    detachDevice( false );
}


int qstreambuf::sync()
{
    if ( iodevice_ && iodevice_->bytesToWrite() )
    {
	if ( !iodevice_->waitForBytesWritten( 10000 ) )
	return -1;
    }

    return 0;
}


std::streambuf::int_type qstreambuf::underflow()
{
    if ( gptr()<egptr() )
	return traits_type::to_int_type( *gptr() );

    char *base = &buffer_.front();
    char *start = base;

    if ( eback()==base )
    {
	memmove( base, egptr() - mPUSH_BACK, mPUSH_BACK );
	start += mPUSH_BACK;
    }

    readAll();

    size_t n = buffer_.size() - (start - base);
    if ( readbuffer_.size()<n )
	n = readbuffer_.size();

    if ( !n )
	return traits_type::eof();

    memmove( start, readbuffer_.data(), n );
    readbuffer_.remove( 0, n );

    setg( base, start, start + n );

    return traits_type::to_int_type( *gptr() );
}


void qstreambuf::detachDevice( bool readall )
{
    if ( readall )
	readAll();

    if ( process_ ) process_->closeWriteChannel();

    if ( ownsdevice_ )
    {
	if ( process_ ) process_->waitForFinished();
	delete iodevice_; 
    }

    iodevice_ = 0;
    process_ = 0;
}


void qstreambuf::readAll()
{
    if ( iodevice_ ) iodevice_->waitForReadyRead( mTIMEOUT );
    QByteArray newdata;
    if ( process_ )
    {
       newdata = isstderr_ ? process_->readAllStandardError()
			   : process_->readAllStandardOutput();
    }
    else if ( iodevice_ )
    {
	newdata  = iodevice_->readAll();
    }

    readbuffer_.append( newdata );
}


std::streamsize qstreambuf::xsputn( const char_type* s, std::streamsize n )
{
    if ( !iodevice_ )
	return 0;

    const std::streamsize res = iodevice_->write( s, n );
    return iodevice_->waitForBytesWritten( mTIMEOUT ) ? res : 0;
}

#endif //No qt
