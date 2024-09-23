#include <string.h>
#include "bitmap.h"

/*Some Common Function used to helping the calculation*/
void get_index_and_mask(u16 value, u16 *index, u32 *mask);
bool is_value_set(struct bitmap *bm, u16 value);
void set_value(struct bitmap *bm, u16 value);
void clear_value(struct bitmap *bm, u16 value);
void update_first_value(struct bitmap *bm);
void update_last_value(struct bitmap *bm);
u8 count_bits_in_byte(u32 byte);
u16 count_set_bits(struct bitmap *bm);

/*************************************************************
 * Name: bitmap_check
 * Input: bm         The bitmap to which values are added
 * Return: Success   true
 *         Failed    false
 * Description: Add a value into the bitmap
 **************************************************************/
static bool bitmap_check(struct bitmap *bm);

void get_index_and_mask(u16 value, u16 *index, u32 *mask)
{
    *index = (value - 1) / UINT_BITS;
    *mask = 1U << ((value - 1) % UINT_BITS);

    return;
}

bool is_value_set(struct bitmap *bm, u16 value)
{
    u16 index = 0;
    u32 mask = 0;

    if (!bitmap_check(bm) || value == 0 || value > bm->max_value)
    {
        return false;
    }

    get_index_and_mask(value, &index, &mask);

    return (bm->buf[index] & mask) != 0;
}

void set_value(struct bitmap *bm, u16 value)
{
    u16 index = 0;
    u32 mask = 0;

    get_index_and_mask(value, &index, &mask);

    bm->buf[index] |= mask;
    
    return;
}

void clear_value(struct bitmap *bm, u16 value)
{
    u16 index = 0;
    u32 mask = 0;

    get_index_and_mask(value, &index, &mask);

    bm->buf[index] &= ~mask;

    return;
}

void update_first_value(struct bitmap *bm)
{
    u32 iteration = 0;

    for (iteration = 1; iteration <= bm->max_value; iteration++)
    {
        if (is_value_set(bm, iteration))
        {
            bm->first_value = iteration;

            return;
        }
    }

    bm->first_value = 0;

    return;
}

void update_last_value(struct bitmap *bm)
{
    u32 iteration = 0;

    for (iteration = bm->max_value; iteration >= 1; iteration--)
    {
        if (is_value_set(bm, iteration))
        {
            bm->last_value = iteration;

            return;
        }
    }

    bm->last_value = 0;

    return;
}

u8 count_bits_in_byte(u32 byte)
{
    u8 count = 0;

    while (byte)
    {
        count += byte & 1;
        byte >>= 1;
    }

    return count;
}

u16 count_set_bits(struct bitmap *bm)
{
    u16 count = 0;
    u16 i = 0;

    for (i = 0; i < bm->buf_len; i++)
    {
        count += count_bits_in_byte(bm->buf[i]);
    }

    return count;
}

struct bitmap *bitMap_create(u16 capacity)
{
    u16 buf_len = 0;
    u16 size = 0;
    struct bitmap *bm = NULL;

    if (capacity == 0)
    {
        return NULL;
    }

    buf_len = (capacity + UINT_BITS - 1) / UINT_BITS;
    size = sizeof(struct bitmap) + buf_len * sizeof(u32);
    bm = (struct bitmap *)calloc(1, size);

    if (bm == NULL)
    {
        return NULL;
    }

    bm->bm_self = bm;
    bm->max_value = capacity;
    bm->first_value = 0;
    bm->last_value = 0;
    bm->numbers = 0;
    bm->buf_len = buf_len;
    memset(bm->buf, 0, buf_len * sizeof(u32));

    return bm;
}

void bitmap_destroy(struct bitmap *bm)
{
    if (bitmap_check(bm))
    {
        bm->bm_self= NULL;
        free(bm);
        bm = NULL;
    }

    return;
}

static bool bitmap_check(struct bitmap *bm)
{
    if (bm != NULL && bm == bm->bm_self)
    {
        return true;
    }

    return false;
}

bool bitmap_add_value(struct bitmap *bm, u16 value)
{
    if (!bitmap_check(bm) || value == 0 || value > bm->max_value)
    {
        return false;
    }

    if (!is_value_set(bm, value))
    {
        set_value(bm, value);
        bm->numbers++;

        if (bm->first_value == 0 || value < bm->first_value)
        {
            bm->first_value = value;
        }

        if (value > bm->last_value)
        {
            bm->last_value = value;
        }
    }

    return true;
}

bool bitmap_del_value(struct bitmap *bm, u16 value)
{
    if (!bitmap_check(bm) || value == 0 || value > bm->max_value)
    {
        return false;
    }

    if (is_value_set(bm, value))
    {
        clear_value(bm, value);
        bm->numbers--;

        if (value == bm->first_value)
        {
            update_first_value(bm);
        }

        if (value == bm->last_value)
        {
            update_last_value(bm);
        }

        return true;
    }

    return false;
}

void bitmap_print(struct bitmap *bm)
{
    u16 start = 0;
    u32 iteration = 0;
    bool in_range = false;
    /* Flag to track whether it's the first number or range being printed*/
    bool first_print = true;

    if (!bitmap_check(bm) || bm->numbers == 0)
    {
        puts("Empty bitmap");

        return;
    }

    start = bm->first_value;
    in_range = false;

    /* Start from first value and ends in maximum value */
    for (iteration = bm->first_value; iteration <= bm->max_value; iteration++)
    {
        /* Check if the value is set */
        if (is_value_set(bm, iteration))
        {
            /* Check if a new range starts */
            if (!in_range)
            {
                start = iteration;
                in_range = true;
            }
        }
        else
        {
            if (in_range)
            {
                /* If only one value */
                if (first_print)
                {
                    first_print = false;
                }
                /* Print a comma before the next element or range */
                else
                {
                    printf(", ");
                }

                if (start == iteration - 1)
                {
                    printf("%hu", start);
                }
                else
                {
                    printf("%hu-%hu", start, iteration - 1);
                }

                in_range = false;
            }
        }
    }

    /* If the last range hasn't been printed */
    if (in_range)
    {
        if (!first_print)
        {
            printf(", ");
        }

        if (start == bm->max_value)
        {
            printf("%hu", start);
        }
        else
        {
            printf("%hu-%hu", start, bm->max_value);
        }
    }

    puts("\n");

    return;
}

struct bitmap *bitmap_clone(struct bitmap *bm)
{
    struct bitmap *new_bm = NULL;

    if (!bitmap_check(bm))
    {
        return NULL;
    }

    new_bm = bitMap_create(bm->max_value);

    if (!bitmap_check(new_bm))
    {
        return NULL;
    }

    memcpy(new_bm, bm, sizeof(struct bitmap) + bm->buf_len * sizeof(u32));
    new_bm->bm_self = new_bm;

    return new_bm;
}

bool bitmap_not(struct bitmap *bm)
{
    u32 iteration = 0;
    u32 extra_bits = 0;
    u32 mask = 0;

    if (!bitmap_check(bm))
    {
        return false;
    }

    for (iteration = 0; iteration < bm->buf_len; iteration++)
    {
        bm->buf[iteration] = ~bm->buf[iteration];
    }

    /* If the buffer length doesn't matches the word size then extra bits need to set*/
    extra_bits = (bm->buf_len * UINT_BITS) - bm->max_value;
    
    if (extra_bits > 0)
    {
        mask = (1U << (UINT_BITS - extra_bits)) - 1;
        bm->buf[bm->buf_len - 1] &= mask;
    }

    bm->numbers = count_set_bits(bm);
    update_first_value(bm);
    update_last_value(bm);

    return true;
}

bool bitmap_or(struct bitmap *bm_store, struct bitmap *bm)
{
    u16 iteration = 0;

    if (!bitmap_check(bm) || !bitmap_check(bm_store))
    {
        return false;
    }

    for (iteration = 0; iteration < bm_store->buf_len; iteration++)
    {
        bm_store->buf[iteration] |= bm->buf[iteration];
    }

    if (bm_store->max_value < bm->max_value)
    {
        u32 extra_bits = (bm_store->buf_len * UINT_BITS) - bm_store->max_value;
        u32 mask = (1U << (UINT_BITS - extra_bits)) - 1;

        /*Set the extra bits in bm_store to 0 in the last buffer element*/
        bm_store->buf[bm_store->buf_len - 1] &= mask;
    }

    bm_store->numbers = count_set_bits(bm_store);
    update_first_value(bm_store);
    update_last_value(bm_store);

    return true;
}

bool bitmap_and(struct bitmap *bm_store, struct bitmap *bm)
{
    u16 iteration = 0;

    if (!bitmap_check(bm) || !bitmap_check(bm_store))
    {
        return false;
    }

    for (iteration = 0; iteration < bm->buf_len; iteration++)
    {
        bm_store->buf[iteration] &= bm->buf[iteration];
    }

    if (bm_store->max_value > bm->max_value)
    {
        memset(bm_store->buf + bm->buf_len, 0, (bm_store->buf_len - bm->buf_len) * sizeof(u32));
    }

    bm_store->numbers = count_set_bits(bm_store);
    update_first_value(bm_store);
    update_last_value(bm_store);

    return true;
}

struct bitmap *bitmap_parse_str(u8 *str)
{
    struct bitmap *bm = NULL;
    char *token = NULL;
    char *dash = NULL;
    u32 start = 0;
    u32 end = 0;
    u32 value = 0;
    u32 iteration = 0;

    if (str == NULL)
    {
        return NULL;
    }

    /*For parsing unsigned short maximum is used*/
    bm = bitMap_create(U16_MAX);

    if (!bitmap_check(bm))
    {
        return NULL;
    }

    /* Using to take range separated by comma*/
    token = strtok((char *)str, ",");

    while (token != NULL)
    {
        dash = strchr(token, '-');

        if (dash)
        {
            start = atoi(token);
            end = atoi(dash + 1);

            /*Returns for invalid inputs*/
            if (start == 0 || end == 0 || start > U16_MAX || end > U16_MAX)
            {
                free(bm);
                bm = NULL;

                return NULL;
            }

            /*Handle Reverse String Also*/
            if (start > end)
            {
                start = start ^ end;
                end = start ^ end;
                start = start ^ end;
            }

            for (iteration = start; iteration <= end; iteration++)
            {
                bitmap_add_value(bm, iteration);
            }
        }
        else
        {
            value = atoi(token);
            
			if(value == 0 || value >U16_MAX)
			{
                free(bm);
                bm = NULL;

                return NULL;				
			}
            
            bitmap_add_value(bm, value);
        }

        token = strtok(NULL, ",");
    }

    return bm;
}
