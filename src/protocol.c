#include "protocol.h"

#include <stdio.h>

#define START_BYTE 0x00
#define END_BYTE 0xFF

typedef enum state
{
    INIT,
    START,
    FLAG,
    ID,
    DATA_LEN,
    DATA,
    END
} state;

static void protocol_print_frame(rf_frame frame);

static rf_frame protocol_frame;
static state current_state;
static uint8_t data_len;
static uint8_t count;
static void (*frame_handler)(rf_frame) = protocol_print_frame;

static void protocol_print_frame(rf_frame frame)
{
    printf("id: %x, flag: %x, len: %x, data: ", frame.id, frame.flag, frame.data_length);
    for (uint8_t i = 0; i < frame.data_length; i++)
    {
        printf("%x", frame.data[i]);
    }
    printf("\n");
}

static void setState(state next_state)
{
    if (next_state != current_state)
    {
        current_state = next_state;
        // printf("State: %d\n", current_state);
    }
}

void protocol_init()
{
    current_state = START;
    data_len = 0;
    count = 0;
    protocol_frame.id = 1;
    protocol_frame.flag = 1;
    protocol_frame.data_length = 0;
}

void protocol_process_byte(uint8_t byte)
{
    // printf("%02x", byte);
    // fflush(stdout);

    switch (current_state)
    {
    case INIT:
        setState(START);
        break;
    case START:
        if (byte == START_BYTE)
            setState(FLAG);
        break;
    case FLAG:
        protocol_frame.flag = byte;
        // printf("flag: %x", byte);
        setState(ID);
        break;
    case ID:
        protocol_frame.id = byte;
        // printf("id: %x", byte);
        setState(DATA_LEN);
        break;
    case DATA_LEN:
        count = 0;
        protocol_frame.data_length = byte;
        // printf("len: %x", byte);
        setState(DATA);
        break;
    case DATA:
        if (count >= protocol_frame.data_length)
            setState(END);
        // printf("%x", byte);
        protocol_frame.data[count] = byte;
        count++;
        break;
    case END:
        if (byte == END_BYTE)
        {
            setState(START);
            frame_handler(protocol_frame);
        }
        break;
    default:
        break;
    }
}

void protocol_handle_frame(void (*frame)(rf_frame))
{
    frame_handler = frame;
}
