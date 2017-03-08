
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include <dlfcn.h>
#include <jni.h>


// uber simple log
#define _LOG( ... ) {\
FILE * fp = fopen("test.log", "a+"); \
printf(__VA_ARGS__); \
fprintf(fp, __VA_ARGS__); \
fclose(fp); }

int main( int argc, char *argv[] )
{
    const char * libjvm="/usr/lib/jvm/java/jre/lib/amd64/server/libjvm.so";
    void * hdl;
    
    _LOG( "Opening %s\n", libjvm );

    if ( !( hdl = dlopen ( libjvm, RTLD_NOW ) ) )
    {
        _LOG( "Failed opening %s\n", dlerror());
        exit(1);
    }
    
    _LOG( "Looking up symbol JNI_CreateJavaVM\n");
    void* (*_fn)() = (void*(*)()) dlsym ( hdl, "JNI_CreateJavaVM" );
    
    if (!_fn)
    {
         _LOG( "Failed symbol look %s\n", dlerror());
        exit(1);
    }
    
    if ( dlclose (hdl) != 0 ) 
    {
        _LOG( "Failed closing %s\n", dlerror());
        exit(1);
    }
    
    _LOG( "SUCCESS\n" );
    exit(0);
}