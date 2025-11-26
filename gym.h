#ifndef GYM_H
#define GYM_H
#include "types.h"
#include "constants.h"   // for num_days, num_focus_areas, etc.

/* Function declarations used everywhere */

void clear_input_buffer();
int get_day_index();

float calculate_bmi(float h, float w);

void load_all_exercises(ExerciseItem db[num_focus_areas][max_exercise_per_category],
                        int counts[num_focus_areas]);

Member* load_data(ExerciseItem db[num_focus_areas][max_exercise_per_category],
                  FoodItem daily_menus[num_days][25]);

void save_data(Member* head);

Member* add_new_member(Member *head);
void display_all_members(Member* head);
Member* search_member(Member* head, char name[]);
void display_member_details(Member* member);
Member* delete_member(Member* head, char name[]);

void add_workout_log(Member* member,
    ExerciseItem db[num_focus_areas][max_exercise_per_category],
    int counts[num_focus_areas]);

void log_food_item(Member* member, FoodItem daily_menus[num_days][25]);
void display_diet_log(Member* member);
void calculate_total_calories(Member* member);

void free_all_memory(Member* head);

#endif
