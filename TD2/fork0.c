#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_COUNT 110
#define BUF_SIZE 50


int main(void)
{
    pid_t pid;
    int i;
    char buf[BUF_SIZE];

    fork();
    pid = getpid();

    for (i = 1; i < MAX_COUNT; i++)
    {
    sprintf(buf, "This line is from pid %d, value = %d\n", pid, i);
    write(1, buf, strlen(buf));
    }
}
