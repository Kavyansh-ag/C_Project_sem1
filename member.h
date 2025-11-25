#ifndef MEMBER_H_
#define MEMBER_H_

#include "diet.h"

#ifndef max_menu_items
#define max_menu_items 25
#endif

#define max_exercise_per_category 10
#define num_focus_areas 5 // Chest, Back, Legs, Shoulders, Arms

typedef struct ExerciseItem {
    int id;
    char name[50];
} ExerciseItem;

typedef struct WorkoutLog {
    int exercise_id;
    char exercise_name[50]; // Copied for easier display
    int sets;
    int reps;
    float weight_used;
    float pr_weight; // Personal Record for this exercise
    struct WorkoutLog *next_workout;
} WorkoutLog;

struct DietLog; //for def refer diet.h
//DietLof is declared here so that Member struct can have pointer to DietLog

#define num_days 7 //constant for days in a week

typedef struct Member {
    char name[50];
    int age;
    char sex[10];
    float height;  // in m
    float weight;  // in kg
    float bmi;
    WorkoutLog *workout_heads[num_days]; // array of head pointers for workout LLs of each day
    struct DietLog *diet_heads[num_days]; // array of head pointers for diet LLs of each day
    struct Member *next_member;
} Member;

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