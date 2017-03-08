#include <stdio.h>
#include "MesosModule.h" 
#include <boost/filesystem.hpp>

void loadModule (const std::string & libpath)
{
    boost::shared_ptr<MesosModule> myModule;
    boost::shared_ptr<SomeBase> plugin_class;

    myModule.reset(new MesosModule());

    if ((plugin_class = myModule->load( libpath.c_str() )))
    {
        plugin_class->does_stuff();
        printf("YAY WE DID STUFF!!!\n");

        if ( 2 == plugin_class->version() ) 
        {
            boost::shared_ptr<SomeDerivedV2> upCastRTTI = boost::dynamic_pointer_cast< SomeDerivedV2> (plugin_class);
            upCastRTTI->does_more_stuff();

            printf("YAY WE DID MORE STUFF!!!\n");
        }

        plugin_class.reset();

    }
    else
    {
        printf("BOO WE SUCK!!!\n");
    }

    printf("***** ABOUT TO CLOSE LIBRARY *****\n");
}


int main( int argc, char* argv[] )
{

    std::string path = boost::filesystem::current_path().string();
    std::string librarypath = path + "/libplugin1.so";

    loadModule(librarypath);

    librarypath = path + "/libplugin2.so";
    loadModule(librarypath);

    return 0;
}
