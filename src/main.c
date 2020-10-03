#include "serial.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

int fd;
bool verbose = false;
char *baud = "9600";
char *filename;

int get_baud(int value)
{
    switch (value)
    {
    case 0:
        return SERIAL_0;
    case 50:
        return SERIAL_50;
    case 75:
        return SERIAL_75;
    case 110:
        return SERIAL_110;
    case 134:
        return SERIAL_134;
    case 150:
        return SERIAL_150;
    case 200:
        return SERIAL_200;
    case 300:
        return SERIAL_300;
    case 600:
        return SERIAL_600;
    case 1200:
        return SERIAL_1200;
    case 1800:
        return SERIAL_1800;
    case 2400:
        return SERIAL_2400;
    case 4800:
        return SERIAL_4800;
    case 9600:
        return SERIAL_9600;
    case 19200:
        return SERIAL_19200;
    case 38400:
        return SERIAL_38400;
    case 57600:
        return SERIAL_57600;
    case 115200:
        return SERIAL_115200;
    case 230400:
        return SERIAL_230400;
    case 460800:
        return SERIAL_460800;
    case 500000:
        return SERIAL_500000;
    case 576000:
        return SERIAL_576000;
    case 921600:
        return SERIAL_921600;
    case 1000000:
        return SERIAL_1000000;
    case 1152000:
        return SERIAL_1152000;
    case 1500000:
        return SERIAL_1500000;
    case 2000000:
        return SERIAL_2000000;
    case 2500000:
        return SERIAL_2500000;
    case 3000000:
        return SERIAL_3000000;
    case 3500000:
        return SERIAL_3500000;
    case 4000000:
        return SERIAL_4000000;
    default:
        return SERIAL_9600;
    }
}

int main(int argc, char **argv)
{
    int opt;

    // Arguemtne parsing strucutre for getopt_long.
    static struct option long_options[] = {
        {"help", no_argument, 0, 0},
        {"verbose", no_argument, 0, 'v'},
        {"baud", required_argument, 0, 'b'},
        {NULL, 0, NULL, 0}};

    opterr = 0;

    // Parses all the arguments.
    while ((opt = getopt_long(argc, argv, "b:v", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'v':
            verbose = true;
            break;
        case 'b':
            baud = optarg;
            break;
        default:
            exit(EXIT_FAILURE);
            break;
        }
    }

    if (optind < argc)
    {
        filename = argv[optind];
    }
    else
    {
        printf("Not enough arguments\n");
    }

    serial_init();
    fd = serial_open(filename);
    serial_set_baud(fd, get_baud(atoi(baud)));
    serial_close(fd);
    return 0;
}
