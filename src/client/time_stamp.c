#include <sys/time.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char tempoA[100], tempoD[100];
    struct timeval antes;
    struct timeval depois;
    int rc;

    rc=gettimeofday(&antes, NULL);
    if(rc==0) {
        printf("gettimeofday() successful.\n");
        sprintf(tempoA, "%u.%06u\n", antes.tv_sec, antes.tv_usec);
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
        sprintf(tempoD, "%u.%06u\n", depois.tv_sec, depois.tv_usec);
        printf("time = %u.%06u\n",
                depois.tv_sec, depois.tv_usec);
    }
    else {
        printf("gettimeofday() failed, errno = %d\n",
                errno);
        return -1;
    }
    printf("Antes: %s", tempoA);
    printf("Depois: %s", tempoD);
    int diff = (depois.tv_sec - antes.tv_sec) * 1000 + (depois.tv_usec - depois.tv_usec) / 1000;
    printf("%d\n", diff);   
    long t = depois.tv_sec * 1000000 + depois.tv_usec;
    printf("%lli\n", t);
    return 0;
}