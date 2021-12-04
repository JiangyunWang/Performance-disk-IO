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

void writeFile(char *file, ssize_t block_size)
{
    int fd = open(file, O_RDONLY, 00700);
    if (fd)
    {

        int amount = 0;
        clock_t begin = clock();
        int result = 0;
        int cnt = 0;
        int *buf = malloc(block_size * sizeof(int));
        printf("Start clock = %ld\n", begin);
        while ((result = read(fd, buf, block_size)) > 0)
        {
            amount += result;
            realloc(buf, block_size * sizeof(int));
            cnt++;
        }

        printf("amount: %d\n", amount);

        clock_t finish = clock();
        double time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
        printf("time spend: %f\n", time_spent);
        printf("The file size is: %d\n", amount);
        printf("End clock = %ld\n", finish);
        printf("B/s: %f\n", amount / time_spent);
        printf("xorbuf: %08x\n", xorbuf(buf, amount));
        amount /= 1000000;

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
