#ifndef UI_AND_INPUT_CONTROL_H_INCLUDED
#define UI_AND_INPUT_CONTROL_H_INCLUDED

#include "bitmap.h"

enum main_menu
{
    CREATE_FIRST_BITMAP = 1,
    CREATE_SECOND_BITMAP,
    SEE_CLONED_BITMAP,
    PARSE_STRING_TO_BITMAP,
    EXIT
};

enum sub_menu
{
    ADD_VALUE = 1,
    REMOVE_VALUE,
    CLONE_BITMAP,
    NOT_BITMAP,
    OR_WITH_SECOND_BITMAP,
    AND_WITH_SECOND_BITMAP,
    DISPLAY_BITMAP,
    BACK_TO_MAIN_MENU
};

void print_bitmap_info_from_ui(struct bitmap *bm, const char *message);
void display_main_menu_from_ui();
void display_submenu_from_ui();
struct bitmap *create_bitmap_from_ui(const char *bitmap_name);
void handle_submenu_from_ui(struct bitmap *bm, struct bitmap *bm_other, struct bitmap **bm_clone);
void see_cloned_bitmap_from_ui(struct bitmap *bm_clone);
void parse_string_to_bitmap_from_ui();
void add_value_to_bitmap_from_ui(struct bitmap *bm);
void remove_value_from_bitmap_from_ui(struct bitmap *bm);
struct bitmap *clone_bitmap_from_ui(struct bitmap *bm);
void perform_not_operation_from_ui(struct bitmap *bm);
void perform_or_operation_from_ui(struct bitmap *bm_store, struct bitmap *bm);
void perform_and_operation_from_ui(struct bitmap *bm_store, struct bitmap *bm);
void menu();

#endif // UI_AND_INPUT_CONTROL_H_INCLUDED
