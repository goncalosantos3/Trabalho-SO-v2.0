#include <sys/time.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    struct timeval antes;
    struct timeval depois;
    int rc;

    rc=gettimeofday(&antes, NULL);
    if(rc==0) {
        printf("gettimeofday() successful.\n");
        printf("time = %u.%06u\n",
                antes.tv_sec, antes.tv_usec);
    }
    else {
        printf("gettimeofday() failed, errno = %d\n",
                errno);
        return -1;
    }

    sleep(3);
    rc=gettimeofday(&depois, NULL);
    if(rc==0) {
        printf("gettimeofday() successful.\n");
        printf("time = %u.%06u\n",
                depois.tv_sec, depois.tv_usec);
    }
    else {
        printf("gettimeofday() failed, errno = %d\n",
                errno);
        return -1;
    }
    int dif = (depois.tv_sec - antes.tv_sec) * 1000;
    printf("Tempo em milisegundos: %d\n", dif);
    return 0;
}