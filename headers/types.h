#ifndef TYPES_H
#define TYPES_H


#include "constants.h"


typedef struct ExerciseItem {
    int id;
    char name[50];
} ExerciseItem;


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


typedef struct WorkoutLog {
    int exercise_id;
    char exercise_name[50]; // Copied for easier display
    int sets;
    int reps;
    float weight_used;
    float pr_weight; // Personal Record for this exercise
    struct WorkoutLog *next_workout;
} WorkoutLog;


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


#endif
