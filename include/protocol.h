#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <stdint.h>

#define FRAME_MAX_DATA_LENGTH 256
#define FRAME_MAX_LEN 263 // 1 + max data length + length of id + 1(data length byte) => 1(start of frame) + 256 + 4 + 1 + 1(end of frame)

// This is a single frame.
// The frame will be sent over the RF transiever.
typedef struct rf_frame
{
    uint8_t flag;
    uint8_t id;
    uint8_t data_length;
    uint8_t data[FRAME_MAX_DATA_LENGTH] __attribute__((aligned(8)));
} rf_frame;

void protocol_init();
void protocol_process_byte(uint8_t byte);
void protocol_handle_frame(void (*frame)(rf_frame));

#endif
