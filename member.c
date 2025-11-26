#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "diet.h"  //member.h is indirectly included in this
// #include "gym.h"
#include "types.h"
#include "member.h"
#include "constants.h"
#include "diet.h"


void load_all_exercises(ExerciseItem db[num_focus_areas][max_exercise_per_category], int counts[num_focus_areas]) {
    FILE *file = fopen("exercise_database.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open exercise_database.txt\n");
        return;
    }

    char line[100];
    int current_part_index = -1;

   // Initialize counts to zero
    for(int i=0; i < num_focus_areas; i++) counts[i] = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Check for category headers
        if (strncmp(line, "[CHEST]", 7) == 0) current_part_index = 0;
        else if (strncmp(line, "[BACK]", 6) == 0) current_part_index = 1;
        else if (strncmp(line, "[LEGS]", 6) == 0) current_part_index = 2;
        else if (strncmp(line, "[ARMS]", 6) == 0) current_part_index = 3;
        else if (strncmp(line, "[SHOULDERS]", 11) == 0) current_part_index = 4;
        else if (current_part_index != -1 && counts[current_part_index] < max_exercise_per_category) {
            // This is a data line for the current category
            int ex_idx = counts[current_part_index];
            // sscanf(line, "%d,%49[^]", &db[current_part_index][ex_idx].id, db[current_part_index][ex_idx].name);
            sscanf(line, "%d,%49[^,]", 
            &db[current_part_index][ex_idx].id,
            db[current_part_index][ex_idx].name);

            counts[current_part_index]++;
        }
    }

    fclose(file);
}

float update_pr(Member* member, int exercise_id, float current_weight) {
    
    float max_pr = current_weight;

    //to check highest PR  we will have to check all days
    for (int i = 0; i < num_days; i++) {
        WorkoutLog* current_log = member->workout_heads[i];
        while (current_log != NULL) {
            if (current_log->exercise_id == exercise_id) {
                if (current_log->weight_used > max_pr) {
                    max_pr = current_log->weight_used;
                }
            }
            current_log = current_log->next_workout;
        }
    }
    return max_pr;
}

Member* add_new_member(Member* head) {

    Member* newNode = (Member*) malloc(sizeof(Member));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed!\n");
        return head; // Return original list
    }

    clear_input_buffer();
    printf("Enter member's name: ");
    fgets(newNode->name, 50, stdin);
    newNode->name[strcspn(newNode->name, "\n")] = 0; // Remove newline

    printf("Enter age: ");
    scanf("%d", &newNode->age);
    clear_input_buffer();

    printf("Enter sex: ");
    fgets(newNode->sex, 10, stdin);
    newNode->sex[strcspn(newNode->sex, "\n")] = 0; // Remove newline
    
    printf("Enter height (in meters): ");
    scanf("%f", &newNode->height);
    clear_input_buffer();

    printf("Enter weight (in kg): ");
    scanf("%f", &newNode->weight);
    clear_input_buffer();

    newNode->bmi = calculate_bmi(newNode->height, newNode->weight);
    printf("Calculated BMI: %.2f\n", newNode->bmi);

    // CRITICAL: Initialize all log head pointers to NULL
    for(int i=0; i < num_days; i++) {
        newNode->workout_heads[i] = NULL;
        newNode->diet_heads[i] = NULL;
    }

    // Link the new member to the front of the list
    newNode->next_member = head;
    printf("Member '%s' added successfully!\n", newNode->name);

    return newNode; // The new node is now the head
}
    
void add_workout_log(Member* member, ExerciseItem db[num_focus_areas][max_exercise_per_category], int counts[num_focus_areas]) {
    if (member == NULL) {
        printf("Error: No member selected.\n");
        return;
    }
    
    int day_index = get_day_index();
    int part_index = get_body_part_index();
    
    char* body_part_names[] = {"Chest", "Back", "Legs", "Arms", "Shoulders"};
    char add_another;

    do {
        printf("\n--- Exercises for %s ---\n", body_part_names[part_index]);
        for (int i = 0; i < counts[part_index]; i++) {
            // Display a 1-based number for the user to select
            printf("  %d: %s\n", i + 1, db[part_index][i].name);
        }

        int choice = -1;
        printf("Select an exercise by number (1-%d): ", counts[part_index]);
        scanf("%d", &choice);
        clear_input_buffer();

        // Validate the user's choice (it's 1-based)
        if (choice < 1 || choice > counts[part_index]) {
            printf("Invalid exercise number.\n");
        } else {
            // Convert 1-based choice to 0-based array index
            ExerciseItem selected_ex = db[part_index][choice - 1];

            WorkoutLog* new_log = (WorkoutLog*) malloc(sizeof(WorkoutLog));
            if (!new_log) {
                printf("Memory allocation failed!\n");
                break; // Exit loop
            }

            printf("Enter sets for %s: ", selected_ex.name);
            scanf("%d", &new_log->sets);
            clear_input_buffer();

            printf("Enter reps: ");
            scanf("%d", &new_log->reps);
            clear_input_buffer();

            printf("Enter weight used (kg): ");
            scanf("%f", &new_log->weight_used);
            clear_input_buffer();
            
            // Set data in the new log node
            new_log->exercise_id = selected_ex.id; // Store the real ID
            strcpy(new_log->exercise_name, selected_ex.name);
            new_log->pr_weight = update_pr(member, selected_ex.id, new_log->weight_used);

            // Add node to the front of the list for that day
            new_log->next_workout = member->workout_heads[day_index];
            member->workout_heads[day_index] = new_log;

            printf("Workout logged successfully! New PR for %s is %.2f kg.\n", new_log->exercise_name, new_log->pr_weight);
        }

        printf("Log another exercise for this session? (y/n): ");
        scanf(" %c", &add_another); // Note the space before %c
        clear_input_buffer();

    } while (add_another == 'y' || add_another == 'Y');
}


void display_workout_plan(Member* member) {
     if (!member) return;
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("\n--- Workout Plan for %s ---\n", member->name);
    
    int workout_found = 0;
    for (int i=0; i < num_days; i++) {
        WorkoutLog* current = member->workout_heads[i];
        if (current != NULL) {
            workout_found = 1;
            printf("\n--- %s ---\n", days[i]);
            printf("  %-20s | Sets | Reps | Weight\n", "Exercise");
            printf("  ------------------------------------------\n");
            while (current != NULL) {
                printf("  %-20s | %-4d | %-4d | %.2f kg\n", current->exercise_name, current->sets, current->reps, current->weight_used);
                current = current->next_workout;
            }
        }
    }
    if (!workout_found) {
        printf("No workouts logged for this member yet.\n");
    }
}

void display_all_members(Member* head) {
    printf("\n--- All Gym Members ---\n");
    if (head == NULL) {
        printf("No members in the system.\n");
        return;
    }
    Member* current = head;
    printf("  %-20s | Age | Sex    | BMI\n", "Name");
    printf("  ------------------------------------------\n");
    while (current != NULL) {
        printf("  %-20s | %-3d | %-6s | %.2f\n", current->name, current->age, current->sex, current->bmi);
        current = current->next_member;
    }
}

Member* search_member(Member* head, const char* name) {
     Member* current = head;
    while (current != NULL) {
        // Use strcmp for string comparison
        if (strcmp(current->name, name) == 0) {
            return current; // Found the member
        }
        current = current->next_member;
    }
    return NULL; // Member not found
}
    
void display_member_details(Member* member) {
    if (member == NULL) {
        printf("Member not found or specified.\n");
        return;
    }
    printf("\n--- Details for Member: %s ---\n", member->name);
    printf("  Age: %d\n", member->age);
    printf("  Sex: %s\n", member->sex);
    printf("  Height: %.2f m\n", member->height);
    printf("  Weight: %.2f kg\n", member->weight);
    printf("  BMI: %.2f\n", member->bmi);

    // Now display their logs
    display_workout_plan(member);
    display_diet_log(member);
}

Member* delete_member(Member* head, const char* name) {
    Member *current = head;
    Member *previous = NULL;

    while (current != NULL && strcmp(current->name, name) != 0) {
        previous = current;
        current = current->next_member;
    }

    if (current == NULL) {
        printf("Member '%s' not found.\n", name);
        return head;
    }

    // Free all sub-lists (workouts and diets) for the member being deleted
    for (int i = 0; i < num_days; i++) {
        WorkoutLog *w_log = current->workout_heads[i];
        while (w_log != NULL) {
            WorkoutLog *temp = w_log;
            w_log = w_log->next_workout;
            free(temp);
        }
        DietLog *d_log = current->diet_heads[i];
        while (d_log != NULL) {
            DietLog *temp = d_log;
            d_log = d_log->next_diet_log;
            free(temp);
        }
    }

    // Unlink the member node from the main list
    if (previous == NULL) { // The member to delete is the head
        head = current->next_member;
    } else {
        previous->next_member = current->next_member;
    }

    free(current); // Free the member struct itself
    printf("Member '%s' has been deleted.\n", name);
    return head;
}

void free_all_memory(Member* head) {
    Member *current_member = head;
    while (current_member != NULL) {
        Member *temp_member = current_member;
        current_member = current_member->next_member; // Move to next before freeing

        // Free all sub-lists for this member
        for (int i = 0; i < num_days; i++) {
            WorkoutLog *w_log = temp_member->workout_heads[i];
            while (w_log != NULL) {
                WorkoutLog *temp_w = w_log;
                w_log = w_log->next_workout;
                free(temp_w);
            }
            DietLog *d_log = temp_member->diet_heads[i];
            while (d_log != NULL) {
                DietLog *temp_d = d_log;
                d_log = d_log->next_diet_log;
                free(temp_d);
            }
        }
        // Finally, free the member struct itself
        free(temp_member);
    }
}