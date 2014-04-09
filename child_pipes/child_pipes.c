#define _GNU_SOURCE             /* See feature_test_macros(7) */

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>


void
child_open_pipe_nhold (int file)
{
    FILE *stream = fdopen (file, "w");

    printf ("Please `kill -s 15 %d` sleeping 10000\n", getpid());

    // pretty much just wait here until it gets killed. 
    sleep (10000);
    fclose(stream);
}


void
parent_wait_on_child (int file)
{
    fd_set set;

    /* Initialize the file descriptor set. */
    FD_ZERO(&set);
    FD_SET(file, &set);

    // add extra file descriptor 
    // could loop through a vector, etc. 

    int ret = select(FD_SETSIZE, &set, NULL, NULL, NULL);

    // Inset checking logic to make it non-dumb

    printf("Ohh no my child must be gone\n");
}

int
main (void)
{
    pid_t pid;
    int mypipe[2];

    /* Create the pipe. */
    if (pipe (mypipe))
    {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    /* Create the child process. */
    pid = fork ();
    if (pid == (pid_t) 0)
    {
        /* This is the child process.
         *        Close other end first. */
        close (mypipe[1]);
        child_open_pipe_nhold (mypipe[0]);

        // we should not get here.

        return EXIT_FAILURE;
    }
    else if (pid < (pid_t) 0)
    {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
    else
    {
        close (mypipe[0]);
        parent_wait_on_child (mypipe[1]);
        return EXIT_SUCCESS;
    }
}