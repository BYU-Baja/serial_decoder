#ifndef __SERIAL_H
#define __SERIAL_H

// Linux headers
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()
#include <stdint.h>

#define SERIAL_0 B0
#define SERIAL_50 B50
#define SERIAL_75 B75
#define SERIAL_110 B110
#define SERIAL_134 B134
#define SERIAL_150 B150
#define SERIAL_200 B200
#define SERIAL_300 B300
#define SERIAL_600 B600
#define SERIAL_1200 B1200
#define SERIAL_1800 B1800
#define SERIAL_2400 B2400
#define SERIAL_4800 B4800
#define SERIAL_9600 B9600
#define SERIAL_19200 B19200
#define SERIAL_38400 B38400
#define SERIAL_57600 B57600
#define SERIAL_115200 B115200
#define SERIAL_230400 B230400
#define SERIAL_460800 B460800
#define SERIAL_500000 B500000
#define SERIAL_576000 B576000
#define SERIAL_921600 B921600
#define SERIAL_1000000 B1000000
#define SERIAL_1152000 B1152000
#define SERIAL_1500000 B1500000
#define SERIAL_2000000 B2000000
#define SERIAL_2500000 B2500000
#define SERIAL_3000000 B3000000
#define SERIAL_3500000 B3500000
#define SERIAL_4000000 B4000000

#define SERIAL_DEFAULT SERIAL_50
#define SERIAL_MIN_BAUD SERIAL_9600
#define SERIAL_MAX_BAUD SERIAL_4000000

void serial_init();

int serial_open(char *filename);

int serial_set_baud(int fd, int baud);

ssize_t serial_read(int fd, uint8_t *buf, int buf_len);

ssize_t serial_write(int fd, uint8_t *buf, int buf_len);

void serial_close(int fd);

#endif
