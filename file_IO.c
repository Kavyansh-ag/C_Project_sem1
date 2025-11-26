#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "file_IO.h"



void save_data(Member* head) {
    FILE* file = fopen("gym_data.txt", "w");
    if (!file) {
        printf("Error: Could not open gym_data.txt for writing.\n");
        return;
    }

    Member* current_member = head;
    while (current_member != NULL) {
        // Save member details
        fprintf(file, "MEMBER,%s,%d,%s,%.2f,%.2f,%.2f\n",
            current_member->name, current_member->age, current_member->sex,
            current_member->height, current_member->weight, current_member->bmi);

        // Save workout logs
        for (int i = 0; i < num_days; i++) {
            WorkoutLog* w_log = current_member->workout_heads[i];
            while (w_log != NULL) {
                fprintf(file, "WORKOUT,%d,%d,%d,%d,%.2f,%.2f\n",
                    i, w_log->exercise_id, w_log->sets, w_log->reps,
                    w_log->weight_used, w_log->pr_weight);
                w_log = w_log->next_workout;
            }
        }

        // Save diet logs
        for (int i = 0; i < num_days; i++) {
            DietLog* d_log = current_member->diet_heads[i];
            while (d_log != NULL) {
                fprintf(file, "DIET,%d,%d,%.1f,%.2f\n",
                    i, d_log->food_id, d_log->quantity_consumed, d_log->total_calories);
                d_log = d_log->next_diet_log;
            }
        }
        current_member = current_member->next_member;
    }

    fclose(file);
    printf("Data saved successfully.\n");
}

Member* load_data(ExerciseItem exercise_db[num_focus_areas][max_exercise_per_category], FoodItem daily_menus[num_days][25]) {
    FILE* file = fopen("gym_data.txt", "r");
    if (!file) {
        printf("No existing save file found. Starting fresh.\n");
        return NULL;
    }

    Member* head = NULL;
    Member* current_member = NULL;
    char line[200];

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // remove newline
        char type[10];
        sscanf(line, "%[^,]", type);

        if (strcmp(type, "MEMBER") == 0) {
            Member* new_member = (Member*)malloc(sizeof(Member));
            sscanf(line, "MEMBER,%[^,],%d,%[^,],%f,%f,%f",
                new_member->name, &new_member->age, new_member->sex,
                &new_member->height, &new_member->weight, &new_member->bmi);

            for (int i = 0; i < num_days; i++) {
                new_member->workout_heads[i] = NULL;
                new_member->diet_heads[i] = NULL;
            }
            new_member->next_member = head;
            head = new_member;
            current_member = new_member;
        }
        else if (strcmp(type, "WORKOUT") == 0 && current_member) {
            WorkoutLog* new_log = (WorkoutLog*)malloc(sizeof(WorkoutLog));
            int day_idx;
            sscanf(line, "WORKOUT,%d,%d,%d,%d,%f,%f",
                &day_idx, &new_log->exercise_id, &new_log->sets, &new_log->reps,
                &new_log->weight_used, &new_log->pr_weight);

            // Find exercise name from DB
            strcpy(new_log->exercise_name, "Unknown");
            for(int i=0; i<num_focus_areas; i++) {
                for(int j=0; j<max_exercise_per_category; j++) {
                    if(exercise_db[i][j].id == new_log->exercise_id) {
                        strcpy(new_log->exercise_name, exercise_db[i][j].name);
                        break;
                    }
                }
            }
            new_log->next_workout = current_member->workout_heads[day_idx];
            current_member->workout_heads[day_idx] = new_log;
        }
        else if (strcmp(type, "DIET") == 0 && current_member) {
            DietLog* new_log = (DietLog*)malloc(sizeof(DietLog));
            int day_idx;
            sscanf(line, "DIET,%d,%d,%f,%f",
                &day_idx, &new_log->food_id, &new_log->quantity_consumed, &new_log->total_calories);

            // Find food name from menu
            strcpy(new_log->food_name, "Unknown");
             if(new_log->food_id > 0 && new_log->food_id <= max_menu_items) {
                strcpy(new_log->food_name, daily_menus[day_idx][new_log->food_id - 1].name);
             }
            
            new_log->next_diet_log = current_member->diet_heads[day_idx];
            current_member->diet_heads[day_idx] = new_log;
        }
    }
    fclose(file);
    printf("Data loaded successfully.\n");
    return head;
}