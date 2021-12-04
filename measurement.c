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

void writeFile(ssize_t block_size)
{
    int fd = open("test.txt", O_RDONLY, 00700);
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
            // result = write(fd, buf, block_size);
            amount += result;
            realloc(buf, block_size * sizeof(int));
            // free(buf);
            // int *buf = malloc(block_size * sizeof(int));
            cnt++;
        }

        // while ((result = write(fd, buf, block_size * block_count)) > 0)
        // {
        //     amount += result;
        // }
        printf("amount: %d\n", amount);

        clock_t finish = clock();
        double time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
        printf("time spend: %f\n", time_spent);
        printf("The file size is: %d\n", amount);
        printf("End clock = %ld\n", finish);
        printf("B/s: %f\n", amount / time_spent);
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
    // printf("size: %d", size);
    writeFile(size);
    return 0;
}
