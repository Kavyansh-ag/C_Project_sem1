#include <stdio.h>
#include <stdlib.h>
#include <string.h>



///////////////////////////////////////////
#include "../headers/diet.h"
#include "../headers/member.h"
#include "../headers/types.h"
#include "../headers/utility_functions.h"
#include "../headers/file_IO.h"
///////////////////////////////////////////



void display_main_menu() {
    printf("\n===== Gym Management System =====\n");
    printf("1. Add New Member\n");
    printf("2. Display All Members\n");
    printf("3. View Member Details\n");
    printf("4. Log Workout\n");
    printf("5. Log Diet\n");
    printf("6. Calculate Daily Calories\n");
    printf("7. Delete Member\n");
    printf("8. Save and Exit\n");
    printf("=================================\n");
    printf("Enter your choice: ");
}



int main() {
    Member* member_list_head = NULL;
    FoodItem daily_menus[num_days][max_menu_items];
    ExerciseItem exercise_database[num_focus_areas][max_exercise_per_category];
    
    // These arrays will store the actual count of items loaded for each category
    int menu_item_counts[num_days] = {0};
    int exercise_counts[num_focus_areas] = {0};

    // --- Phase 1: Initialization ---
    printf("Loading databases...\n");
    load_food_database(daily_menus, menu_item_counts);
    load_all_exercises(exercise_database, exercise_counts);
    member_list_head = load_data(exercise_database, daily_menus);
    printf("Initialization complete!\n");

    int choice;
    char search_name[50];
    Member* selected_member = NULL;

    // --- Phase 2: Main Loop ---
    while (1) {
        display_main_menu();
        scanf("%d", &choice);
        clear_input_buffer();

        switch (choice) {
            case 1: // Add New Member
                member_list_head = add_new_member(member_list_head);
                break;
            case 2: // Display All Members
                display_all_members(member_list_head);
                break;
            case 3: // View Member Details
                printf("Enter name of member to view: ");
                fgets(search_name, 50, stdin);
                search_name[strcspn(search_name, "\n")] = 0;
                selected_member = search_member(member_list_head, search_name);
                if (selected_member) {
                    display_member_details(selected_member);
                } else {
                    printf("Member not found.\n");
                }
                break;
            case 4: // Log Workout
                printf("Enter name of member to log workout for: ");
                fgets(search_name, 50, stdin);
                search_name[strcspn(search_name, "\n")] = 0;
                selected_member = search_member(member_list_head, search_name);
                if (selected_member) {
                    add_workout_log(selected_member, exercise_database, exercise_counts);
                } else {
                    printf("Member not found.\n");
                }
                break;
            case 5: // Log Diet
                printf("Enter name of member to log diet for: ");
                fgets(search_name, 50, stdin);
                search_name[strcspn(search_name, "\n")] = 0;
                selected_member = search_member(member_list_head, search_name);
                if (selected_member) {
                    log_food_item(selected_member, daily_menus, menu_item_counts);
                } else {
                    printf("Member not found.\n");
                }
                break;
            case 6: // Calculate Calories
                 printf("Enter name of member to calculate calories for: ");
                fgets(search_name, 50, stdin);
                search_name[strcspn(search_name, "\n")] = 0;
                selected_member = search_member(member_list_head, search_name);
                if (selected_member) {
                    calculate_total_calories(selected_member);
                } else {
                    printf("Member not found.\n");
                }
                break;
            case 7: // Delete Member
                printf("Enter name of member to delete: ");
                fgets(search_name, 50, stdin);
                search_name[strcspn(search_name, "\n")] = 0;
                member_list_head = delete_member(member_list_head, search_name);
                break;
            case 8: // Save and Exit
                save_data(member_list_head);
                free_all_memory(member_list_head);
                printf("Exiting program. Goodbye!\n");
                return 0; // Exit successfully
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0; // Should not be reached
}