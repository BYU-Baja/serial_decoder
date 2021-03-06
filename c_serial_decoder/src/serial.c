#include "serial.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SUCCESS 1

void serial_init()
{
}

int serial_open(char *filename)
{
    int fd = open(filename, O_RDWR);

    if (fd < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    return fd;
}

int serial_set_baud(int fd, int baud)
{
    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if (tcgetattr(fd, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;                                                        // Disable echo
    tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    if (tcgetattr(fd, &tty) != 0)
    {
        printf("Error %d from tcgetattr: %s\n", errno, strerror(errno));
    }

    cfsetispeed(&tty, baud);
    cfsetospeed(&tty, baud);

    return SUCCESS;
}

ssize_t serial_read(int fd, uint8_t *buf, int buf_len)
{
    return read(fd, buf, buf_len);
}
ssize_t serial_write(int fd, uint8_t *buf, int buf_len)
{
    return write(fd, buf, buf_len);
}
void serial_close(int fd)
{
    close(fd);
}