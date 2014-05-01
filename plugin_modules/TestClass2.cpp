#include "boost/shared_ptr.hpp"
#include "MyFakeInterface.h"
#include <stdio.h>

// Here is my rocking plugin class
class MyPluginClassV2 : public SomeDerivedV2
{
public:
    MyPluginClassV2():SomeDerivedV2() 
    {
        printf("MyPluginClassV2 is ALIVE!\n");
    }

    virtual ~MyPluginClassV2()
    {
        printf("MyPluginClassV2 is DEADBEEF\n");
    }

    virtual void does_stuff()
    {
        printf("MyPluginClassV2[%d] does_stuff\n", this->version());
    }

    virtual void does_more_stuff()
    {
        printf("See I told you MyPluginClassV2 does_more_stuff\n");
    }
};

// extern is to ensure signature name is not mangled
// and visible via dlsym
extern "C"
{
    boost::shared_ptr< SomeBase > magikMagikSymbolYo( void * foo )
    {
        boost::shared_ptr< SomeBase > ret; 
        ret.reset(new MyPluginClassV2(/*foo*/));

        return (ret);
    }
}