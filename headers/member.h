#ifndef MEMBER_H
#define MEMBER_H

#include "constants.h"
#include "types.h"

//member functions:
Member* add_new_member(Member* head);
void display_all_members(Member* head);
Member* search_member(Member* head, const char* name);
void display_member_details(Member* member);
Member* delete_member(Member* head, const char* name);
void free_all_memory(Member* head);
//------------------------------------------------------

//workout functions:
void load_all_exercises(ExerciseItem db[num_focus_areas][max_exercise_per_category], int counts[num_focus_areas]);
void add_workout_log(Member* member, ExerciseItem db[num_focus_areas][max_exercise_per_category], int counts[num_focus_areas]);
void display_workout_plan(Member* member);
float update_pr(Member* member, int exercise_id, float current_weight);
//-------------------------------------------

//utility functions:
float calculate_bmi(float height, float weight);
void clear_input_buffer();
int get_day_index();
int get_body_part_index();
//----------------------------------------------------

//I/O functions:
void save_data(Member* head);
Member* load_data(ExerciseItem exercise_db[num_focus_areas][max_exercise_per_category], struct FoodItem dail_menus[num_days][max_menu_items]);
//-------------------------------------------------

#endif // MEMBER_H