#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"
#include "input-handler.h"

void flush_input_buffer()
{
    u32 ch = 0;

    while ((ch = getchar()) != '\n' && ch != EOF);

    return;
}

u8 *validate_and_process_input()
{
    char input[MAX_LENGTH_OF_STR] = {0};
    u8 *trimmed = NULL;
    u32 trimmed_len = 0;
    u16 i = 0;
    u16 dash_count = 0;
    u16 comma_count = 0;
    bool prev_char_is_digit = false;

    trimmed = (u8 *)calloc(MAX_LENGTH_OF_STR + 1, sizeof(char));

    if (trimmed == NULL)
    {
        return NULL;
    }

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        puts("Error reading input.");
        free(trimmed);
        trimmed = NULL;

        return NULL;
    }

    if (input[strlen(input) - 1] != '\n')
    {
        flush_input_buffer();
        puts("Warning: Input exceeded 255 characters.\n");

        return NULL;
    }

    /* Remove any trailing newline character */
    input[strcspn(input, "\n")] = '\0';

    for (i = 0; i < MAX_LENGTH_OF_STR && input[i] != '\0'; i++)
    {
        if (!isspace(input[i]))
        {
            trimmed[trimmed_len++] = input[i];
        }
    }

    trimmed[trimmed_len] = '\0';

    for (i = 0; trimmed[i] != '\0'; i++)
    {
        if (isdigit(trimmed[i]))
        {
            prev_char_is_digit = true;
        }
        else if (trimmed[i] == '-')
        {
            /* If dash is present without a preceding digit, it's invalid */
            if (!prev_char_is_digit)
            {
                free(trimmed);
                trimmed = NULL;

                return NULL;
            }

            dash_count++;

            /* Only allow one dash between numbers */
            if (dash_count > 1)
            {
                free(trimmed);
                trimmed = NULL;

                return NULL;
            }

            prev_char_is_digit = false;
        }
        else if (trimmed[i] == ',')
        {
            /* If comma is present without a preceding digit, it's invalid */
            if (!prev_char_is_digit)
            {
                free(trimmed);
                trimmed = NULL;

                return NULL;
            }

            dash_count = 0;
            comma_count++;
            prev_char_is_digit = false;
        }
        else
        {
            /* Invalid character detected */
            free(trimmed);
            trimmed = NULL;

            return NULL;
        }
    }

    /* Check if the last character was a digit */
    if (!prev_char_is_digit)
    {
        free(trimmed);
        trimmed = NULL;

        return NULL;
    }

    return trimmed;
}

bool get_value(u16 *choice)
{
    char buffer[BUFFER_SIZE] = {0};
    u16 index = 0;
    u32 value = 0;
    u16 start_of_char = 0;

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
    {
        puts("Input cancelled. Try Again.");

        return false;
    }

    if (buffer[strlen(buffer) - 1] != '\n')
    {
        flush_input_buffer();
        puts("Warning: Input exceeded maximum characters.\n");

        return false;
    }
    else
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }

    /* Skip leading spaces */
    while (buffer[start_of_char] == ' ')
    {
        start_of_char++;
    }

    for (index = start_of_char; buffer[index] != '\0'; index++)
    {
        if (!isdigit(buffer[index]))
        {
            puts("Non-digit characters present. Try Again.");

            return false;
        }

        /* If a space is detected after a digit, truncate the input to first digit*/
        if (buffer[index + 1] == ' ' && buffer[index + 1] != '\0')
        {
            buffer[index + 1] = '\0';

            break;
        }
    }

    value = atoi(buffer + start_of_char);

    if (value > U16_MAX)
    {
        puts("Number out of range. Try Again.");

        return false;
    }

    *choice = (u16)value;

    return true;
}
