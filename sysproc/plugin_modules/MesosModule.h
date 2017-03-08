
#ifndef __MESOS_MODULE__
#define __MESOS_MODULE__

#include <string> 
#include "DynamicLibrary.h"
#include "MyFakeInterface.h"
#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"


// in mesos proper.. 

// So the separation around symbol semantics is important.
// otherwise the encapsulation becomes fuzzy.

// Below are utility functions which gives an implied context
// a single f(n) will return a shared ptr;

// You can really template the f(n) signature & params too
// plus have all kinds of loading patterns and policies
// but I'm keeping it simple here, but giving the idea.

template <class T>
boost::shared_ptr<T> instantiate_singleton_class( boost::shared_ptr <DynamicLibrary> pLib,
                                           const char * pszMagikSymbol,
                                           void * optional_args=0 )
{
    boost::function< boost::shared_ptr<T> ( void * ) > MyNewClassFn;
    boost::shared_ptr<T> ret;
    void * pSymbol=0;

    if (0 == pLib->sym(std::string(pszMagikSymbol) , pSymbol))
    {
        MyNewClassFn  = boost::bind( reinterpret_cast< boost::shared_ptr<T> (*)(void *) > (pSymbol), _1 );
        ret = MyNewClassFn(optional_args);
    }

    return ret;
}

//////////////////////////////////////////
class MesosModule
{
public:
    MesosModule(){;}
    virtual ~MesosModule(){;}

    virtual boost::shared_ptr<SomeBase> load( const std::string & path )
    {
        pDynamicLib.reset(new DynamicLibrary());

        if ( 0 == pDynamicLib->open(path) ) 
        {
            pImpl = instantiate_singleton_class<SomeBase>(pDynamicLib, "magikMagikSymbolYo");
        }

        return pImpl;
    };

protected:

    // order matters on destruction
    boost::shared_ptr<DynamicLibrary> pDynamicLib;

    // this could be a list.
    boost::shared_ptr<SomeBase> pImpl;
};

#endif
