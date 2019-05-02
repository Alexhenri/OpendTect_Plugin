#ifndef strmdata_h
#define strmdata_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	A.H.Bril
 Date:		3-4-1996
 Contents:	Data on any stream
 RCS:		$Id$
________________________________________________________________________

-*/

#include "basicmod.h"
#include "bufstring.h"
#include <iosfwd>


/*!
\brief Holds data to use and close an iostream.

  Usually created by StreamProvider.
  Need to find out what to do with the pipe in windows.
*/

mExpClass(Basic) StreamData
{
public:

		StreamData()			{ initStrms(); }
    void	transferTo(StreamData&);	//!< retains file name

    void	close();
    bool	usable() const;

    void	setFileName( const char* fn )	{ fname_ = fn; }
    const char*	fileName() const		{ return fname_; }

    std::ios*	streamPtr()const;

    std::istream* istrm;
    std::ostream* ostrm;

protected:

    BufferString fname_;

private:

    inline void	initStrms()
		{ istrm = 0; ostrm = 0; }
    friend class StreamProvider;

};


#endif
