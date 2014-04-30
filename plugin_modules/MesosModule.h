

#ifndef __MESOS_MODULE__
#define __MESOS_MODULE__

#include "boost/noncopyable.hpp"


//////////////////////////////////////////
// highlander approach to class loading (there can be only one!)
// not always true, but sometimes nice when coupled with forking
// that way it ensures each "Module Load" has a unique 
class SomeBase : public boost::noncopyable 
{
public:

    SomeBase() : m_version(1) {;}
    virtual ~SomeBase() {;}

    virtual void does_stuff() = 0;

    load

    virtual int version()
    { return m_version; }

protected:
    int m_version;
}

//////////////////////////////////////////
class SomeDerivedV2 : public class SomeBase
{
public:

    SomeDerivedV2() : m_version(2) {;}
    virtual ~SomeDerivedV2() {;}

    virtual void does_more_stuff() = 0;

protected:
    int m_magik_private_stuff;
}


//////////////////////////////////////////
template <class T> 
class MesosModule
{
    friend class MesosModuleLoader;

protected:

    // order matters on destruction
    boost::shared_ptr<DynamicLibrary> pDynamicLib;

    // this could be a list.
    boost::shared_ptr<T> pImpl;
}

//////////////////////////////////////////
template <class T> 
class MesosModuleLoader
{
public:
    static boost::shared_ptr<T> load(const std::string & szSomePath);

    // you could add all your other utility functions.

private:
    static std::vector< MesosModule<T> > m_modules;

}



#endif
