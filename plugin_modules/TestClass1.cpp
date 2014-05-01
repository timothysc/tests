
#include "boost/shared_ptr.hpp"
#include "MyFakeInterface.h"
#include <stdio.h>

// Here is my rocking plugin class
class MyPluginClassV1 : public SomeBase
{
public:
    MyPluginClassV1():SomeBase()
    {
        printf("MyPluginClassV1 is ALIVE!\n");
    }

    virtual ~MyPluginClassV1()
    {
        printf("MyPluginClassV1 is DEADBEEF\n");
    }

    virtual void does_stuff()
    {
        printf("MyPluginClassV1[%d] does_stuff\n", this->version());
    }
};

// extern is to ensure signature name is not mangled
// and visible via dlsym
extern "C" {
    boost::shared_ptr< SomeBase > magikMagikSymbolYo( void * foo )
    {
        boost::shared_ptr< SomeBase > ret; 
        ret.reset(new MyPluginClassV1(/*foo*/));

        return (ret);
    }
}