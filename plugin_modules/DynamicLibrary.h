
#ifndef __DYNAMIC_LIBRARY__
#define __DYNAMIC_LIBRARY__

#include <string>
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
            this->close();
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


// So the separation around symbol semantics is important.
// otherwise the encapsulation becomes fuzzy.

// Below are utility functions which gives an implied context
// a single f(n) will return a shared ptr;

// You can really template the f(n) signature & params too
// but were going to plaid here

template <class T>
shared_ptr<T> instantiate_singleton_class( shared_ptr <DynamicLibrary> pLib,
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


#endif



