
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
// #include "io.c"
unsigned int xorResult = 0;
unsigned int xorbuf(int *buffer, int size)
{

    for (int i = 0; i < size; i++)
    {
        xorResult ^= buffer[i];
    }
    return xorResult;
}

void writeFile(char *file, ssize_t block_size)
{
    int fd = open(file, O_RDONLY, 00700);
    if (fd)
    {

        double amount = 0;

        double result = 0;
        int *buf = malloc(block_size);
        clock_t begin = clock();
        while ((result = read(fd, buf, block_size)) > 0)
        {
            amount += result;
            xorbuf(buf, block_size);
        }

        // printf("amount: %d\n", amount);

        clock_t finish = clock();
        double time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;

        printf("Start clock = %ld\n", begin);
        printf("End clock = %ld\n", finish);
        printf("time spend: %f\n\n", time_spent);
        printf("The file size is: %f\n", amount);
        printf("B/s: %f\n", amount / time_spent);
        printf("xorbuf: %08x\n", xorResult);
        amount /= 1048576;

        printf("Mib/s: %f\n", amount / time_spent);
        free(buf);
    }
    else
    {
        printf("Invalid file");
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    int size = atoi(argv[1]);
    writeFile(argv[2], size);
    return 0;
}
