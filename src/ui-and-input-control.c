#include <stdio.h>
#include <stdlib.h>
#include "ui-and-input-control.h"
#include "input-handler.h"
#include "bitmap.h"

void print_bitmap_info_from_ui(struct bitmap *bm, const char *message)
{
    if (bm == NULL)
    {
        puts("Bitmap not initialized.");

        return;
    }

    printf("\n%s\n", message);
    printf("Max Number It Can Hold: %d\n", bm->max_value);
    printf("First value: %d\n", bm->first_value);
    printf("Last value: %d\n", bm->last_value);
    printf("Number of set bits: %d\n", bm->numbers);
    printf("Bitmap content: ");
    bitmap_print(bm);

    return;
}

void display_main_menu_from_ui()
{
    puts("+----------------------------------------------+");
    puts("|              Bitmap Main Menu                |");
    puts("+----------------------------------------------+");
    puts("| 1. Operation of First Bitmap                 |");
    puts("| 2. Operation of Second Bitmap                |");
    puts("| 3. See Cloned Bitmap                         |");
    puts("| 4. Parse String to Bitmap                    |");
    puts("| 5. Exit                                      |");
    puts("+----------------------------------------------+");

    return;
}

void display_submenu_from_ui()
{
    puts("+----------------------------------------------+");
    puts("|          Bitmap Operations Submenu           |");
    puts("+----------------------------------------------+");
    puts("| 1. Add Value                                 |");
    puts("| 2. Remove Value                              |");
    puts("| 3. Clone this Bitmap                         |");
    puts("| 4. NOT this Bitmap                           |");
    puts("| 5. OR with Other Bitmap                      |");
    puts("| 6. AND with Other Bitmap                     |");
    puts("| 7. Display This Bitmap                       |");
    puts("| 8. Back to Main Menu                         |");
    puts("+----------------------------------------------+");

    return;
}

struct bitmap *create_bitmap_from_ui(const char *bitmap_name)
{
    u16 capacity = 0;
    struct bitmap *bm = NULL;

    do
    {
        printf("Enter the Capacity for the %s Bitmap: ", bitmap_name);
    }
    while (get_value(&capacity) != true);

    bm = bitMap_create(capacity);

    if (bm != NULL)
    {
        printf("%s Bitmap created successfully.\n", bitmap_name);
    }
    else
    {
        printf("Failed to create %s Bitmap.\n", bitmap_name);
    }

    return bm;
}

void see_cloned_bitmap_from_ui(struct bitmap *bm_clone)
{
    if (bm_clone != NULL)
    {
        print_bitmap_info_from_ui(bm_clone, "Cloned bitmap:");
    }
    else
    {
        puts("No cloned bitmap available.");
    }

    return;
}

void parse_string_to_bitmap_from_ui()
{
    struct bitmap *bm_parsed = NULL;
    u8 *result = NULL;

    printf("Give a string upto 255 characters (i,e 1-14,15,16-77,70-14): ");
    result = validate_and_process_input();

    if (result == NULL)
    {
        puts("Invalid input.");

        return;
    }

    bm_parsed = bitmap_parse_str(result);
    free(result);
    result = NULL;

    if (bm_parsed == NULL)
    {
        puts("Failed to parse string to bitmap.");

        return;
    }

    print_bitmap_info_from_ui(bm_parsed, "Parsed bitmap:");
    bitmap_destroy(bm_parsed);
    bm_parsed = NULL;

    return;
}

void add_value_to_bitmap_from_ui(struct bitmap *bm)
{
    u16 value = 0;

    do
    {
        printf("Enter value to add: ");
    }
    while (get_value(&value) != true);

    if (bitmap_add_value(bm, value))
    {
        printf("Value %hu added successfully.\n", value);

        return;
    }

    printf("Failed to add value %hu.\n", value);

    return;
}

void remove_value_from_bitmap_from_ui(struct bitmap *bm)
{
    u16 value = 0;

    do
    {
        printf("Enter value to remove: ");
    }
    while (get_value(&value) != true);

    if (bitmap_del_value(bm, value))
    {
        printf("Value %hu removed successfully.\n", value);

        return;
    }

    printf("Failed to remove value %hu.\n", value);

    return;
}

struct bitmap *clone_bitmap_from_ui(struct bitmap *bm)
{
    return bitmap_clone(bm);
}

void perform_not_operation_from_ui(struct bitmap *bm)
{
    if (bitmap_not(bm))
    {
        puts("NOT operation performed successfully.");

        return;
    }

    puts("Failed to perform NOT operation.");

    return;
}

void perform_or_operation_from_ui(struct bitmap *bm_store, struct bitmap *bm)
{
    if (bitmap_or(bm_store, bm))
    {
        puts("OR operation performed successfully.");

        return;
    }

    puts("Failed to perform OR operation.");

    return;
}

void perform_and_operation_from_ui(struct bitmap *bm_store, struct bitmap *bm)
{
    if (bitmap_and(bm_store, bm))
    {
        puts("AND operation performed successfully.");

        return;
    }

    puts("Failed to perform AND operation.");

    return;
}

void handle_submenu_from_ui(struct bitmap *bm, struct bitmap *bm_other, struct bitmap **bm_clone)
{
    u16 choice = 0;

    do
    {
        display_submenu_from_ui();
        
		do
	    {
	        printf("Enter your choice: ");
	    }
	    while (get_value(&choice) != true);
	    
        switch (choice)
        {
            case ADD_VALUE:
                add_value_to_bitmap_from_ui(bm);
                break;
            case REMOVE_VALUE:
                remove_value_from_bitmap_from_ui(bm);
                break;
            case CLONE_BITMAP:
            	if(*bm_clone != NULL)
            	{
		    free(*bm_clone);
	            *bm_clone = NULL;	
		}

                *bm_clone = clone_bitmap_from_ui(bm);
                
                if (*bm_clone != NULL)
                {
                    puts("Bitmap cloned successfully.");
                }
                else
                {
                    puts("Failed to clone bitmap.");
                }
                
                break;
            case NOT_BITMAP:
                perform_not_operation_from_ui(bm);
                break;
            case OR_WITH_SECOND_BITMAP:
                perform_or_operation_from_ui(bm, bm_other);
                break;
            case AND_WITH_SECOND_BITMAP:
                perform_and_operation_from_ui(bm, bm_other);
                break;
            case DISPLAY_BITMAP:
                print_bitmap_info_from_ui(bm, "Current Bitmap:");
                break;
            case BACK_TO_MAIN_MENU:
                return;
            default:
                puts("Invalid choice. Please try again.");
                break;
        }
    }
    while (choice != BACK_TO_MAIN_MENU);

    return;
}

void menu()
{
    struct bitmap *bm_first  = NULL;
    struct bitmap *bm_second = NULL;
    struct bitmap *bm_clone  = NULL;
    u16 choice = 0;

    do
    {
        display_main_menu_from_ui();
        
		do
	    {
	        printf("Enter your choice: ");
	    }
	    while (get_value(&choice) != true);

        switch (choice)
        {
            case CREATE_FIRST_BITMAP:
                if (bm_first == NULL)
                {
                    bm_first = create_bitmap_from_ui("First");
                    handle_submenu_from_ui(bm_first, bm_second, &bm_clone);
                }
                else
                {
                    handle_submenu_from_ui(bm_first, bm_second, &bm_clone);
                }
                
                break;
            case CREATE_SECOND_BITMAP:
                if (bm_second == NULL)
                {
                    bm_second = create_bitmap_from_ui("Second");
                    handle_submenu_from_ui(bm_second, bm_first, &bm_clone);
                }
                else
                {
                    handle_submenu_from_ui(bm_second, bm_first, &bm_clone);
                }
                
                break;
            case SEE_CLONED_BITMAP:
                see_cloned_bitmap_from_ui(bm_clone);
                break;
            case PARSE_STRING_TO_BITMAP:
                parse_string_to_bitmap_from_ui();
                break;
            case EXIT:
                puts("Exiting...");
                bitmap_destroy(bm_first);
                bitmap_destroy(bm_second);
                bitmap_destroy(bm_clone);
                break;
            default:
                puts("Invalid choice. Please try again.");
                break;
        }
    }
    while (choice != EXIT);

    return;
}
