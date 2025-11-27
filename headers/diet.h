#ifndef DIET_H
#define DIET_H

#include "constants.h"
#include "types.h"

//Menu Functions:
void load_food_database(FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]);

//DietLog Functions:
void log_food_item(Member* member, FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]);
void display_diet_log(Member* member);
void calculate_total_calories(Member* member);

#endif // DIET_H_