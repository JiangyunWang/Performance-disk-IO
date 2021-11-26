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



void errorMessage(char *s){
    perror(s);
    exit(1);
}

int main(int argc, char* argv[])
{
    //  ./run <filename> [-r|-w] <block_size> <block_count>

    if (argc!=5) {
        errorMessage("Invalid argument amount.\n It should be: ./run <filename> [-r|-w] <block_size> <block_count>");
    }

    ssize_t block_size = atoi(argv[3]);
    ssize_t block_count = atoi(argv[4]);
    if (block_size<=0 || block_count<=0) errorMessage("Invalid block amount");

    // ???  malloc  or  char[]
    char* buf = malloc(block_size*block_count*sizeof(char));
    size_t result;
    float amount = 0;

    FILE *fd;
    printf("block_size = %zd and block_count= %zd\n ", block_size, block_count);
    if (argv[2][0]=='-' && argv[2][1]=='r') { 
         fd =fopen(argv[1], "r");
        if (fd) {
            // use clock for precison clock
            clock_t begin = clock();
            printf("Start clock = %s", ctime(&begin));
            while (!feof(fd)) {
            result = fread(buf, block_size, block_count, fd);
            amount+=result;
            // printf("result: %zu\n", result);
            }
            
            clock_t finish = clock();
            double time_spent = (double)(finish - begin) / CLOCKS_PER_SEC;
            printf("End clock = %s", ctime(&finish));
            printf("B/s: %f\n", amount/time_spent);
            amount/=1000000;
            

            printf("Mib/s: %f\n", amount/time_spent);
            free(buf);

        } else {
            errorMessage("Invalid file");
        }
    } 
    // else if (argv[2][0]=='-'&&argv[2][1]=='w') {
    //     fd =fopen(argv[1], "w");
    //     if (fd) {
    //         time(&start);
    //         printf("Start time = %s", ctime(&start));
    //         while (!feof(fd)) {
    //         result = fwrite(buf, block_size, block_count, fd);
    //         // printf("result: %zu\n", result);
    //         }

    //         time(&end);
    //         printf("End time = %s", ctime(&end));
    //         free(buf);

    //     } else {
    //         errorMessage("Invalid file");
    //     }

    // } else  {
    //     errorMessage("Invalid input");
    // }


   return 0;
}

