#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sched.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>

#define _LOG( ... ) {\
FILE * fp = fopen("namespace.txt", "a+"); \
fprintf(fp, __VA_ARGS__); \
fclose(fp); }

/////////////////////////////////////////////////////////
// output the contents of a directory for checking 
// contents to verify if it's mounted or not.
void output_dir(char * pszDir)
{
    DIR *dp;
    struct dirent *ep;     
    dp = opendir (pszDir);

    _LOG( "[%d] Contents of %s\n", getpid(), pszDir);
    
    if (dp != NULL)
    {
        while (ep = readdir (dp))
            _LOG( "%s\n", ep->d_name );
    
        closedir (dp);
    }
    else
        perror ("Couldn't open the directory");
}

/////////////////////////////////////////////////////////
// main entry point
int main(int argc, char ** argv)
{

	int ret = 0;

    _LOG( "[%d] STARTING \n", getpid());
    
	// fork fuse mount test application
	int pid = fork();
	if (!pid) 
    {        
        // create a new process mount namespace
        // don't muck with the parents, it could leak!
        ret = unshare(CLONE_NEWNS);
        if (ret) {
            _LOG("FAILED TO UNSHARE\n");
            return errno;
        }
        
        _LOG( "[%d] UNSHARE COMPLETE\n", getpid());
        
        // recursively mount everything below / as slave in new pid-namespace
        // this should override shared propagation 
        ret = mount("", "/", "dontcare", MS_REC|MS_SLAVE, "");
        if (ret) {
            _LOG( "failed to mark / rslave for\n" );
            return errno;
        }
        
        _LOG( "[%d] SLAVE MOUNT COMPLETE\n", getpid());
        
        // Now you can perform any mounts below to your hearts content
        // In our case we will kick a script that does a fuse mount
        char *argv[2];
        argv[0] = strdup("testfuse.sh");
        argv[1] = NULL;
        execv("testfuse.sh", argv);
        
		_exit(errno);
	} else if (pid == -1) {
		ret = errno;
	} else {
		int status;
		errno = EINVAL;
		ret = waitpid(pid, &status, 0);
		ret = ret == pid ? 0 : -1;
		if (!ret) {
			ret = status;
		}
		
		output_dir("./fusemount_test/");
		
	}	

	_LOG ("[%d] RET=%d\n",getpid(), ret);

	return ret;

}

