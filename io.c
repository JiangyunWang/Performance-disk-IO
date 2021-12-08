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

void errorMessage(char *s)
{
    perror(s);
    exit(1);
}

unsigned int xorbuf(unsigned int *buffer, int size)
{
    unsigned int result = 0;
    for (int i = 0; i < size; i++)
    {
        result ^= buffer[i];
    }

    return result;
}

void readFile(int fd, int block_size, int block_count)
{
    double amount = 0;
    int cnt = 0;
    unsigned int xorResult;
    clock_t begin = clock();
    unsigned int buf[block_size];
    while (cnt < block_count)
    {
        result = read(fd, buf, block_size);
        xorResult ^= xorbuf(buf, block_size);

        amount += result;
        cnt++;
        if (result <= 0)
            break;
    }

    clock_t finish = clock();
    time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
    printf("File size: %f Bytes\n", amount);
    printf("Time spend is %f sec\n", time_spent);
    printf("xorbuf: %08x\n", xorResult);
    printf("B/s: %f\n", amount / time_spent);

    amount /= 1024 * 1024;

    printf("Mib: %f\n", amount);
    printf("Mib/s: %f\n", amount / time_spent);
    close(fd);
}

void writeFile(int fd, int block_size, int block_count)
{
    unsigned int buf[block_size];
    double amount = 0;
    int cnt = 0;

    for (int i = 1; i < block_size; i++)
    {
        buf[i] = buf[i - 1] * 31 + 17;
    }
    clock_t begin = clock();
    while (cnt < block_count)
    {
        lseek(fd, 0, SEEK_END);
        result = write(fd, buf, block_size);
        amount += result;
        cnt++;
    }
    clock_t finish = clock();
    time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
    printf("File size: %f Bytes\n", amount);
    printf("Time spend is %f sec\n", time_spent);
    printf("B/s: %f\n", amount / time_spent);
    amount /= 1024 * 1024;
    printf("Mib: %f\n", amount);
    printf("Mib/s: %f\n", amount / time_spent);
    close(fd);
}

int main(int argc, char *argv[])
{
    //  ./run <filename> [-r|-w] <block_size> <block_count>
    if (argc != 5)
    {
        errorMessage("Invalid argument amount.\n It should be: ./run <filename> [-r|-w] <block_size> <block_count>");
    }

    int block_size = atoi(argv[3]);
    int block_count = atoi(argv[4]);

    if (block_size <= 0 || block_count <= 0)
        errorMessage("Invalid block amount");

    int fd;
    if (argv[2][0] == '-' && argv[2][1] == 'r')
    {
        fd = open(argv[1], O_RDONLY, 00700);
        if (fd)
        {

            readFile(fd, block_size, block_count);
        }
        else
        {
            errorMessage("Invalid file");
        }
    }
    else if (argv[2][0] == '-' && argv[2][1] == 'w')
    {
        fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd)
        {

            writeFile(fd, block_size, block_count);
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
