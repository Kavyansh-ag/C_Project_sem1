#ifndef DIET_H_
#define DIET_H_

#include "member.h"

#define max_menu_items 25

typedef struct FoodItem {
    int food_id;  // 1, 2, 3... within each day's menu
    char name[50];
    char unit[20]; // e.g., "1 piece", "cup", "1 serving", bowl
    float calories_per_unit;
} FoodItem;

typedef struct DietLog {
    int food_id; // References a FoodItem
    char food_name[50]; // Copied from database file for display
    float quantity_consumed;
    float total_calories;
    struct DietLog *next_diet_log;
} DietLog;

//Menu Functions:
void load_food_database(FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]);

//DietLog Functions:
void log_food_item(Member* member, FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]);
void display_diet_log(Member* member);
void calculate_total_calories(Member* member);

#endif // DIET_H_