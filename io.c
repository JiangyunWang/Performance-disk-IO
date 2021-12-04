// #include <bits/floatn-common.h>
// #include <bits/types/time_t.h>
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

double time_spent;
int result = 0;
// int amount = 0;
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

void readFile(int fd, int *buf, int block_size, int block_count, double amount)
{
    // int amount = 0;
    // use clock for precison clock
    clock_t begin = clock();
    printf("Start clock = %ld\n", begin);
    int cnt = 0;
    while (cnt < block_count)
    {
        result = read(fd, buf, block_size);
        amount += result;
        cnt++;
        if (result <= 0)
            break;
    }
    printf("amount: %d\n", amount);

    clock_t finish = clock();
    time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
    printf("End clock = %ld\n", finish);
    printf("B/s: %f\n", amount / time_spent);
    printf("xorbuf: %08x\n", xorbuf(buf, amount));
    amount /= 1048576;
    printf("Mib: %f\n", amount);
    printf("Mib/s: %f\n", amount / time_spent);
    free(buf);
    close(fd);
}

void writeFile(int fd, int *buf, int block_size, int block_count, double amount)
{
    // use clock for precison clock
    clock_t begin = clock();
    int cnt = 0;
    printf("Start clock = %ld\n", begin);
    while (cnt < block_count)
    {
        result = write(fd, buf, block_size);
        amount += result;
        cnt++;
    }

    printf("amount: %d\n", amount);

    clock_t finish = clock();
    time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
    printf("End clock = %ld\n", finish);
    printf("B/s: %f\n", amount / time_spent);
    amount /= 1000000;

    printf("Mib/s: %f\n", amount / time_spent);
    free(buf);
    close(fd);
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

    int *buf = malloc(block_size * block_count * sizeof(int));

    int fd;
    // printf("block_size = %zd and block_count= %zd\n ", block_size, block_count);
    if (argv[2][0] == '-' && argv[2][1] == 'r')
    {
        fd = open(argv[1], O_RDONLY, 00700);
        if (fd)
        {
            readFile(fd, buf, block_count, block_size, 0);
        }
        else
        {
            errorMessage("Invalid file");
        }
    }
    else if (argv[2][0] == '-' && argv[2][1] == 'w')
    {
        for (int i = 0; i < block_size * block_count; i++)
        {
            buf[i] = 'a';
        }
        fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd)
        {

            writeFile(fd, buf, block_count, block_size, 0);
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
