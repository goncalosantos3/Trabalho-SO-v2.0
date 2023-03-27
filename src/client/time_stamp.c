#include <sys/time.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    struct timeval now;
    int rc;

    rc=gettimeofday(&now, NULL);
    if(rc==0) {
        printf("gettimeofday() successful.\n");
        printf("time = %u.%06u\n",
                now.tv_sec, now.tv_usec);
    }
    else {
        printf("gettimeofday() failed, errno = %d\n",
                errno);
        return -1;
    }

    return 0;
}