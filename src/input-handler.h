#ifndef INPUT_HANDLER_H_INCLUDED
#define INPUT_HANDLER_H_INCLUDED

#include "bitmap.h"

#define MAX_LENGTH_OF_STR 256
#define BUFFER_SIZE 7

void flush_input_buffer();
bool get_value(u16 *choice);
u8 *validate_and_process_input();

#endif // INPUT_HANDLER_H_INCLUDED
