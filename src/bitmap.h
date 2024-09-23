#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <limits.h>

#define UINT_BITS (sizeof(uint32_t)*CHAR_BIT)
#define U16_MAX 65535

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef enum
{
    false = 0,
    true
} bool;

struct bitmap
{
    struct bitmap *bm_self; /* The value used when creating a bitmap */
    u16 max_value;          /* The value used when creating a bitmap */
    u16 first_value;        /* The first bit has been set */
    u16 last_value;         /* The last bit has been set */
    u16 numbers;            /* Number of '1' bits in buf[] */
    u16 buf_len;
    u32 buf[0]; /* Flexible array member for bitmap storage */
};

/*****************************************************************************************************
 * Name: bitMap_create
 * Input:  capacity  The capacity of the bitmap that will be created
 * Return: Success   pointer to bitmap
 *         Failed    NULL
 * Description: Create a new bitmap
 *****************************************************************************************************/
struct bitmap *bitMap_create(u16 capacity);

/*****************************************************************************************************
 * Name: bitmap_destroy
 * Input: bm        A bitmap that will be destroyed
 * Return: Success  None
 *         Fail     Remind(None)
 * Description: Destroy a bitmap
 *****************************************************************************************************/
void bitmap_destroy(struct bitmap *bm);

/*************************************************************
 * Name: bitmap_add_value
 * Input: bm         The bitmap to which values are added
 *        value      A value that will be added into the bitmap
 * Return: Success   true
 *         Failed    false
 * Description: Add a value into the bitmap
 **************************************************************/
bool bitmap_add_value(struct bitmap *bm, u16 value);

/************************************************************************
 * Name: bitmap_del_value
 * Input: bm        The bitmap from which values are removed
 *        value     A value that will be removed from the specified bitmap
 * Return: Success  true
 *         Failed   false
 * Description: Remove a value from the specified bitmap
 ************************************************************************/
bool bitmap_del_value(struct bitmap *bm, u16 value);

/*******************************************************
 * Name: bitmap_print
 * Input: bm      A bitmap that will be printed
 * Return: None
 * Description: Print all the elements in the bitmap
 *******************************************************/
void bitmap_print(struct bitmap *bm);

/*********************************************************************
 * Name: bitmap_clone
 * Input: bm         A bitmap that will be copied
 * Return: Success  None
 *         Fail     None
 * Description: Create a new bitmap and copy specified bm data into it
 **********************************************************************/
struct bitmap *bitmap_clone(struct bitmap *bm);

/*********************************************************************
 * Name: bitmap_not
 * Input: bm         A bitmap that will be reversed with all the binary bits
 * Return: Success   true
 *         Failed    false
 * Description: Reverse all the binary bits
 **********************************************************************/
bool bitmap_not(struct bitmap *bm);

/****************************************************************************************
 * Name: bitmap_or
 * Input:
 *    bm_store       A bitmap that participates in binary OR operations and stores the results
 *    bm             Another bitmap that participates in binary OR operations
 * Return: Success   true
 *         Failed    false
 * Description: Perform binary OR operation (bm_store | bm)
 *****************************************************************************************/
bool bitmap_or(struct bitmap *bm_store, struct bitmap *bm);

/******************************************************************************************
 * Name: bitmap_and
 * Input:
 *    bm_store       A bitmap that participates in binary AND operations and stores the results
 *    bm             Another bitmap that participates in binary AND operations
 * Return: Success   true
 *         Failed    false
 * Description: Perform binary AND operation (bm_store & bm)
 *******************************************************************************************/
bool bitmap_and(struct bitmap *bm_store, struct bitmap *bm);

/******************************************************************************************
 * Name: bitmap_parse_str
 * Input: str       A string that will be parsed to a bitmap
 * Return: Success  Pointer to A new bitmap and stores the data is parsed from the specified string
 *         Failed   NULL
 * Description: Parse a string to create a bitmap
 *******************************************************************************************/
struct bitmap *bitmap_parse_str(u8 *str);

/*Used for simplicity and modularity following prototype are used*/
/*****************************************************************************************************
 * Name: get_index_and_mask
 * Input:  value  The value for which index and mask will be calculated
 *         index  Pointer to store the calculated index
 *         mask   Pointer to store the calculated mask
 * Return: Success None
 *         Failed  None
 * Description: Calculate the index and mask for the given value in the bitmap
 *****************************************************************************************************/
void get_index_and_mask(u16 value, u16 *index, u32 *mask);

/*****************************************************************************************************
 * Name: is_value_set
 * Input:  bm     Pointer to the bitmap structure
 *         value  The value to check in the bitmap
 * Return: Success   true
 *         Failed    false
 * Description: Check if a given value is set in the bitmap
 *****************************************************************************************************/
bool is_value_set(struct bitmap *bm, u16 value);

/*****************************************************************************************************
 * Name: set_value
 * Input:  bm     Pointer to the bitmap structure
 *         value  The value to set in the bitmap
 * Return: Success None
 *         Failed  None
 * Description: Set the given value in the bitmap
 *****************************************************************************************************/
void set_value(struct bitmap *bm, u16 value);

/*****************************************************************************************************
 * Name: clear_value
 * Input:  bm     Pointer to the bitmap structure
 *         value  The value to clear in the bitmap
 * Return: Success None
 *         Failed  None
 * Description: Clear the given value from the bitmap
 *****************************************************************************************************/
void clear_value(struct bitmap *bm, u16 value);

/*****************************************************************************************************
 * Name: update_first_value
 * Input:  bm     Pointer to the bitmap structure
 * Return: Success None
 *         Failed  None
 * Description: Update the first value set in the bitmap, setting it to 0 if no values are set
 *****************************************************************************************************/
void update_first_value(struct bitmap *bm);

/*****************************************************************************************************
 * Name: update_last_value
 * Input:  bm     Pointer to the bitmap structure
 * Return: Success None
 *         Failed  None
 * Description: Update the last value set in the bitmap, setting it to 0 if no values are set
 *****************************************************************************************************/
void update_last_value(struct bitmap *bm);

/*****************************************************************************************************
 * Name: count_set_bits
 * Input:  bm     Pointer to the bitmap structure
 * Return: Success   count value
 *         Failed    0
 * Description: Count the number of set bits in the bitmap
 *****************************************************************************************************/
u16 count_set_bits(struct bitmap *bm);

#endif // BITMAP_H_INCLUDED
