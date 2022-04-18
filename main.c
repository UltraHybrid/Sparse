#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int size = 4096;
    int flags, opt;
    int nsecs, tfnd;
    nsecs = 0;
    tfnd = 0;
    flags = 0;
    while ((opt = getopt(argc, argv, "b:")) != -1) {
        switch (opt) {
            case 'b':
                size = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "Использование: %s [-b nsecs] имя\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char *file_in;
    char *file_out;
    int source_desc = 0;
    int out_desc;
    int d = argc - optind;
    if (d == 2)
    {
        file_in = argv[optind++];
        file_out = argv[optind];
        source_desc = open(file_in, O_RDWR);
    }
    if (d == 1)
    {
        source_desc = STDIN_FILENO;
        file_out = argv[optind];
    }

    if(source_desc < 0)
    {
        perror("error open source file1");
        return source_desc;
    }
    out_desc = open(file_out, O_CREAT | O_RDWR, S_IRWXO | S_IRWXU);
    if(out_desc < 0)
    {
        perror("error open result file");
        return out_desc;
    }
    ftruncate(out_desc, 0);
    char buffer[size];
    int total_size = 0;

    int res = read(source_desc, buffer, size);
    if(res < 0)
    {
        perror("error read source file");
        return res;
    }
    while(res > 0)
    {
        int flag = 0;
        for(int i = 0; i < size; ++i)
        {
            if(buffer[i] == '1')
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            int lseek1 = lseek(out_desc, res, SEEK_CUR);
            if(lseek1 < 0)
            {
                perror("error lseek");
                return lseek1;
            }

            total_size += res;
        }
        else
        {
            int a = write(out_desc, buffer, res) < 0;
            if(a)
            {
                perror("error write");
                return a;
            }

            total_size += res;
        }

        res = read(source_desc, buffer, size);
        if(res < 0)
        {
            perror("error read source file");
            return res;
        }

    }
    ftruncate(out_desc, total_size);
    close(source_desc);
    close(out_desc);
    return 0;
}
