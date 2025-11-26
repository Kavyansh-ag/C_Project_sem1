#ifndef GYM_H
#define GYM_H
#include "types.h"
#include "constants.h"   // for num_days, num_focus_areas, etc.

// #define num_days 7
// #define num_focus_areas 6
// #define max_exercise_per_category 20

// typedef struct WorkoutLog {
//     int exercise_id;
//     char exercise_name[50];
//     int sets;
//     int reps;
//     float weight_used;
//     float pr_weight;
//     struct WorkoutLog* next_workout;
// } WorkoutLog;

// typedef struct DietLog {
//     char food_name[50];
//     float calories;
//     struct DietLog* next_diet_log;
// } DietLog;

// typedef struct ExerciseItem {
//     int id;
//     char name[50];
//     char muscle_group[50];
// } ExerciseItem;

// typedef struct FoodItem {
//     char name[50];
//     float calories;
// } FoodItem;

// typedef struct Member {
//     char name[50];
//     int age;
//     char sex[10];
//     float height;
//     float weight;
//     float bmi;

//     WorkoutLog* workout_heads[num_days];
//     DietLog* diet_heads[num_days];

//     struct Member* next_member;
// } Member;

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
