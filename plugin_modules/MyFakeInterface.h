#ifndef __MYFAKE_INTERFACE__
#define __MYFAKE_INTERFACE__

#include "boost/noncopyable.hpp"

//////////////////////////////////////////
// highlander approach to class loading (there can be only one!)
// not always true, but sometimes nice when coupled with forking
// that way it ensures each "Module Load" has a unique 

// Note once you create your interface you need to maintain your api
// or else your vtable is fubar.
class SomeBase : public boost::noncopyable 
{
public:

    SomeBase(int version=1) : m_version(version) {;}
    virtual ~SomeBase() {;}

    virtual void does_stuff() = 0;

    virtual int version()
    { return m_version; }

protected:
    int m_version;
};

//////////////////////////////////////////
class SomeDerivedV2 : public SomeBase
{
public:

    SomeDerivedV2() :SomeBase(2) {;}
    virtual ~SomeDerivedV2() {;}

    virtual void does_more_stuff() = 0;

protected:
    int m_magik_private_stuff;
};

#endif
