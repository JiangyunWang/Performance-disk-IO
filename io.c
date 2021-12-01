#include <bits/floatn-common.h>
#include <bits/types/time_t.h>
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

void errorMessage(char *s)
{
    perror(s);
    exit(1);
}

unsigned int xorbuf(int *buffer, int size)
{
    unsigned int result = 0;
    for (int i = 0; i < size; i++)
    {
        result ^= buffer[i];
    }
    return result;
}

int main(int argc, char *argv[])
{
    //  ./run <filename> [-r|-w] <block_size> <block_count>

    if (argc != 5)
    {
        errorMessage("Invalid argument amount.\n It should be: ./run <filename> [-r|-w] <block_size> <block_count>");
    }

    ssize_t block_size = atoi(argv[3]);
    ssize_t block_count = atoi(argv[4]);
    if (block_size <= 0 || block_count <= 0)
        errorMessage("Invalid block amount");

    ssize_t result;
    int amount = 0;
    int *buf = malloc(block_size * block_count * sizeof(int));

    int fd;
    // printf("block_size = %zd and block_count= %zd\n ", block_size, block_count);
    if (argv[2][0] == '-' && argv[2][1] == 'r')
    {
        fd = open(argv[1], O_RDONLY);
        if (fd)
        {
            // use clock for precison clock
            clock_t begin = clock();
            printf("Start clock = %s", ctime(&begin));
            while (result = read(fd, buf, block_size * block_count) > 0)
            {
                amount += result;
            }
            // printf("result: %zu\n", result);

            clock_t finish = clock();
            double time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
            printf("End clock = %s", ctime(&finish));
            printf("B/s: %f\n", amount / time_spent);
            printf("xorbuf: %08x\n", xorbuf(buf, amount));
            amount /= 1000000;

            printf("Mib/s: %f\n", amount / time_spent);
            free(buf);
            close(fd);
        }
        else
        {
            errorMessage("Invalid file");
        }
    }
    else if (argv[2][0] == '-' && argv[2][1] == 'w')
    {
        for (int i = 0; i < amount; i++)
        {
            buf[i] = 'a';
        }
        fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd)
        {
            // use clock for precison clock
            clock_t begin = clock();
            printf("Start clock = %s", ctime(&begin));
            result = write(fd, buf, amount);
            // printf("result: %zu\n", result);

            clock_t finish = clock();
            double time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
            printf("End clock = %s", ctime(&finish));
            printf("B/s: %f\n", amount / time_spent);
            amount /= 1000000;

            printf("Mib/s: %f\n", amount / time_spent);
            free(buf);
            close(fd);
        }
        else
        {
            errorMessage("Invalid file");
        }
    }
    else
    {
        errorMessage("Invalid input");
    }

    return 0;
}
