#ifndef manobjectset_h
#define manobjectset_h

/*+
________________________________________________________________________

 (C) dGB Beheer B.V.; (LICENSE) http://opendtect.org/OpendTect_license.txt
 Author:	Bert
 Date:		Feb 2009
 RCS:		$Id$
________________________________________________________________________

-*/

#include "objectset.h"


/*!
\brief ObjectSet where the objects contained are owned by this set.
*/

template <class T>
mClass(Basic) ManagedObjectSet : public ObjectSet<T>
{
public:

    typedef int			size_type;
    typedef T			object_type;

    inline			ManagedObjectSet()	{}
    inline			ManagedObjectSet(const ManagedObjectSet<T>&);
    inline virtual		~ManagedObjectSet();
    inline ManagedObjectSet<T>&	operator =(const ObjectSet<T>&);
    inline ManagedObjectSet<T>&	operator =(const ManagedObjectSet<T>&);
    virtual bool		isManaged() const	{ return true; }

    inline virtual void		erase()			{ deepErase( *this ); }
    inline virtual void		append(const ObjectSet<T>&);
    inline virtual void		removeRange(size_type,size_type);
    inline virtual T*		removeSingle( int idx, bool kporder=true );
				/*!<Deletes entry and returns 0 */
    inline virtual T*		removeAndTake(int idx, bool kporder=true );
				/*!<Does not delete the entry. */
    inline virtual T*		replace(int idx, T*);
				/*!<Deletes entry and returns 0 */
    inline virtual ManagedObjectSet<T>& operator -=(T*);

};


//ObjectSet implementation

template <class T> inline
ManagedObjectSet<T>::ManagedObjectSet( const ManagedObjectSet<T>& t )
    : ObjectSet<T>()
{ *this = t; }

template <class T> inline
ManagedObjectSet<T>::~ManagedObjectSet()
{ erase(); }


template <class T> inline
ManagedObjectSet<T>& ManagedObjectSet<T>::operator =( const ObjectSet<T>& os )
{
    if ( &os != this )
	{ erase(); append( os ); }
    return *this;
}


template <class T> inline
ManagedObjectSet<T>& ManagedObjectSet<T>::operator -=( T* ptr )
{
    if ( ptr )
	{ this->vec_.erase( (void*)ptr ); delete ptr; }
    return *this;
}


template <class T> inline
ManagedObjectSet<T>& ManagedObjectSet<T>::operator =(
					const ManagedObjectSet<T>& os )
{
    if ( &os != this )
	deepCopy( *this, os );
    return *this;
}


template <class T> inline
void ManagedObjectSet<T>::append( const ObjectSet<T>& os )
{
    const int sz = os.size();
    this->vec_.setCapacity( this->size()+sz, true );
    if ( !os.isManaged() )
	ObjectSet<T>::append( os );
    else
	for ( int idx=0; idx<sz; idx++ )
	    ObjectSet<T>::add( os[idx] ? new T( *os[idx] ) : 0 );
}


template <class T> inline
T* ManagedObjectSet<T>::removeSingle( int idx, bool kporder )
{
    delete (*this)[idx];
    ObjectSet<T>::removeSingle( idx, kporder );
    return 0; //Don't give anyone a chance to play with the deleted object
}


template <class T> inline
T* ManagedObjectSet<T>::replace( int idx , T* ptr )
{
    delete ObjectSet<T>::replace( idx, ptr );
    return 0; //Don't give anyone a chance to play with the deleted object
}


template <class T> inline
void ManagedObjectSet<T>::removeRange( size_type i1, size_type i2 )
{
    for ( int idx=(int)i1; idx<=i2; idx++ )
	delete (*this)[idx];
    ObjectSet<T>::removeRange( i1, i2 );
}

template <class T> inline
T* ManagedObjectSet<T>::removeAndTake(int idx, bool kporder )
{
    return ObjectSet<T>::removeSingle( idx, kporder );
}


#endif
