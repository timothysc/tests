
#ifndef __DYNAMIC_LIBRARY__
#define __DYNAMIC_LIBRARY__

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// Stout code 
class DynamicLibrary
{
  public:

    DynamicLibrary()
    { m_hdl = 0; };

    virtual ~DynamicLibrary()
    {
        if ( m_hdl )
        {
            printf("Closing Library\n");
            this->close();
        }
    };

   int open( const std::string & szModulePath )
   {
       int ret = 0;
       if (!( m_hdl = dlopen ( szModulePath.c_str(), RTLD_NOW ) ) )
       {
           ret = EXIT_FAILURE;
           printf("Error opening : %s\n", dlerror());
       }
       return ret;
   }

   int close()
   {
       int ret;
       if ( (ret = dlclose (m_hdl)) != 0 ) 
       {
           printf("Error closing : %s\n", dlerror());
       }

       return ret;
   }

   int sym( const std::string & szSymbol, void *& pSymbol )
   {
       int ret = 0;
       if (!(pSymbol =  dlsym ( m_hdl, szSymbol.c_str() ) ))
       {
           ret = EXIT_FAILURE;
           printf("Error looking up symbol %s : %s\n", 
                  szSymbol.c_str(), dlerror());
       }
       return ret;
   }

  private:
    void * m_hdl;

};

#endif



