#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diet.h"
#include "types.h"
#include "utility_functions.h"


void load_food_database(FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]) {
    FILE* file = fopen("food_database.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open food_database.txt\n");
        return;
    }

    char line[100];
    int current_day_index = -1;

    // Initialize counts to zero
    for(int i=0; i<num_days; i++) menu_item_counts[i] = 0;

    while(fgets(line, sizeof(line), file)) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = 0;

        if(strncmp(line, "[SUNDAY]", 8) == 0) current_day_index = 0;
        else if(strncmp(line, "[MONDAY]", 8) == 0) current_day_index = 1;
        else if(strncmp(line, "[TUESDAY]", 9) == 0) current_day_index = 2;
        else if(strncmp(line, "[WEDNESDAY]", 11) == 0) current_day_index = 3;
        else if(strncmp(line, "[THURSDAY]", 10) == 0) current_day_index = 4;
        else if(strncmp(line, "[FRIDAY]", 8) == 0) current_day_index = 5;
        else if(strncmp(line, "[SATURDAY]", 10) == 0) current_day_index = 6;
        else if (current_day_index != -1 && menu_item_counts[current_day_index] < max_menu_items) {
            int item_idx = menu_item_counts[current_day_index];
            sscanf(line, "%49[^,],%19[^,],%f", 
                daily_menus[current_day_index][item_idx].name,
                daily_menus[current_day_index][item_idx].unit,
                &daily_menus[current_day_index][item_idx].calories_per_unit);
            
            daily_menus[current_day_index][item_idx].food_id = item_idx + 1; // Assign simple ID
            menu_item_counts[current_day_index]++;
        }
    }
    fclose(file);
}
    

void log_food_item(Member* member, FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]) {
    if (!member) {
        printf("Error: No member selected.\n");
        return;
    }

    int day_index = get_day_index();
    
    while(1) { // Loop for logging multiple items in one session
        printf("\n--- Menu for the Day ---\n");
        for (int i = 0; i < menu_item_counts[day_index]; i++) {
            // Display a 1-based number for the user to select
            printf("  %d: %s (%s)\n", i + 1, daily_menus[day_index][i].name, daily_menus[day_index][i].unit);
        }

        int choice;
        printf("\nEnter food item number to log (or 0 to finish): ");
        scanf("%d", &choice);
        clear_input_buffer();

        if (choice == 0) {
            printf("Finished logging diet for this session.\n");
            break; // Exit the logging loop
        }

        // Validate the user's choice (it's 1-based)
        if (choice < 1 || choice > menu_item_counts[day_index]) {
            printf("Invalid item number. Please try again.\n");
            continue; // Skip to the next iteration of the loop
        }
        
        // Convert 1-based choice to 0-based array index
        FoodItem* selected_item = &daily_menus[day_index][choice - 1];
        
        float quantity;
        printf("Enter quantity for %s (e.g., if unit is '1 piece' and you ate 2, enter 2): ", selected_item->name);
        scanf("%f", &quantity);
        clear_input_buffer();

        DietLog* new_log = (DietLog*) malloc(sizeof(DietLog));
        if(!new_log) {
            printf("Memory allocation failed!\n");
            return; // Exit the function
        }

        // Populate the new log node
        new_log->food_id = selected_item->food_id;
        strcpy(new_log->food_name, selected_item->name);
        new_log->quantity_consumed = quantity;
        new_log->total_calories = quantity * selected_item->calories_per_unit;

        // Add to the front of the member's diet list for that day
        new_log->next_diet_log = member->diet_heads[day_index];
        member->diet_heads[day_index] = new_log;

        printf("Logged: %.1f x %s (%.2f calories)\n", quantity, new_log->food_name, new_log->total_calories);
    }
}

void display_diet_log(Member* member) {
    if (!member) return;
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("\n--- Diet Log for %s ---\n", member->name);

    int food_found = 0;
    for (int i=0; i < num_days; i++) {
        DietLog* current = member->diet_heads[i];
        if (current != NULL) {
            food_found = 1;
            printf("\n--- %s ---\n", days[i]);
            printf("  %-25s | Quantity | Calories\n", "Food Item");
            printf("  ----------------------------------------------\n");
            while (current != NULL) {
                printf("  %-25s | %-8.1f | %.2f\n", current->food_name, current->quantity_consumed, current->total_calories);
                current = current->next_diet_log;
            }
        }
    }
    if (!food_found) {
        printf("No diet items logged for this member yet.\n");
    }
}

void calculate_total_calories(Member* member) {
    if (!member) return;
    
    printf("Which day would you like to calculate total calories for?\n");
    int day_index = get_day_index();
    
    float total_cals = 0.0f;
    DietLog* current = member->diet_heads[day_index];
    while(current != NULL) {
        total_cals += current->total_calories;
        current = current->next_diet_log;
    }
    
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("\nTotal calories consumed by %s on %s: %.2f kcal\n", member->name, days[day_index], total_cals);
}