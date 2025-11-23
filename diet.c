#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diet.h"

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
        if(strncmp(line, "[SUNDAY]", 8) == 0) current_day_index = 0;
        else if(strncmp(line, "[MONDAY]", 8) == 0) current_day_index = 1;
        else if(strncmp(line, "[TUESDAY]", 9) == 0) current_day_index = 2;
        else if(strncmp(line, "[WEDNESDAY]", 11) == 0) current_day_index = 3;
        else if(strncmp(line, "[THURSDAY]", 10) == 0) current_day_index = 4;
        else if(strncmp(line, "[FRIDAY]", 8) == 0) current_day_index = 5;
        else if(strncmp(line, "[SATURDAY]", 10) == 0) current_day_index = 6;
        else if (current_day_index != -1 && menu_item_counts[current_day_index] < max_menu_items) {
            int item_idx = menu_item_counts[current_day_index];
            sscanf(line, "%49[^,],%9[^,],%f", 
                daily_menus[current_day_index][item_idx].name,
                daily_menus[current_day_index][item_idx].unit,
                &daily_menus[current_day_index][item_idx].calories_per_unit);
                // char *p = line;
            // char *comma;
            // size_t len;

            // /* Default values (match original behavior: count increments even on partial/failed parses) */
            // daily_menus[current_day_index][item_idx].name[0] = '\0';
            // daily_menus[current_day_index][item_idx].unit[0] = '\0';
            // daily_menus[current_day_index][item_idx].calories_per_unit = 0.0f;

            // /* parse name (up to first comma). If no comma, name is the whole line)
            //    and we'll leave unit empty and calories 0 to mimic previous sscanf behavior */
            // comma = strchr(p, ',');
            // if (!comma) {
            //     /* take whole line as name (trim trailing newline) */
            //     len = strlen(p);
            //     if (len > 0 && p[len - 1] == '\n') len--;
            //     if (len > 49) len = 49;
            //     strncpy(daily_menus[current_day_index][item_idx].name, p, len);
            //     daily_menus[current_day_index][item_idx].name[len] = '\0';
            // } else {
            //     len = (size_t)(comma - p);
            //     if (len > 49) len = 49;
            //     strncpy(daily_menus[current_day_index][item_idx].name, p, len);
            //     daily_menus[current_day_index][item_idx].name[len] = '\0';

            //     /* parse unit (between first and second comma). If no second comma,
            //        unit is the rest of the line (trim newline) and calories remain 0 */
            //     p = comma + 1;
            //     comma = strchr(p, ',');
            //     if (!comma) {
            //         len = strlen(p);
            //         if (len > 0 && p[len - 1] == '\n') len--;
            //         if (len > 9) len = 9;
            //         strncpy(daily_menus[current_day_index][item_idx].unit, p, len);
            //         daily_menus[current_day_index][item_idx].unit[len] = '\0';
            //     } else {
            //         len = (size_t)(comma - p);
            //         if (len > 9) len = 9;
            //         strncpy(daily_menus[current_day_index][item_idx].unit, p, len);
            //         daily_menus[current_day_index][item_idx].unit[len] = '\0';

            //         /* parse calories (rest of line). If conversion fails, leave as 0 */
            //         p = comma + 1;
            //         char *endptr;
            //         float cal = strtof(p, &endptr);
            //         if (p != endptr) {
            //             daily_menus[current_day_index][item_idx].calories_per_unit = cal;
            //         }
            //     }
            // }
            daily_menus[current_day_index][item_idx].food_id = item_idx + 1; // Assign ID
            menu_item_counts[current_day_index]++;
        }
    }
    fclose(file);
}

void log_food_item(Member* member, FoodItem daily_menus[num_days][max_menu_items], int menu_item_counts[num_days]) {
    if (member == NULL) {
        printf("Error: No member selected.\n");
        return;
    }

    int day_index = get_day_index();
    char add_another;

    do {
        printf("\n--- Menu for this Day ---\n");
        for (int i = 0; i < menu_item_counts[day_index]; i++) {
            printf("  %d: %s (%.2f kcal/%s)\n", 
                daily_menus[day_index][i].food_id, 
                daily_menus[day_index][i].name, 
                daily_menus[day_index][i].calories_per_unit, 
                daily_menus[day_index][i].unit);
        }

        int choice = -1;
        printf("Select a food item by ID: ");
        scanf("%d", &choice);
        clear_input_buffer();
        
        if (choice < 1 || choice > menu_item_counts[day_index]) {
            printf("Invalid food ID.\n");
        } else {
            FoodItem selected_food = daily_menus[day_index][choice - 1];
            
            DietLog* new_log = (DietLog*) malloc(sizeof(DietLog));
            if (!new_log) {
                printf("Memory allocation failed!\n");
                break;
            }

            printf("Enter quantity consumed (in %s): ", selected_food.unit);
            scanf("%f", &new_log->quantity_consumed);
            clear_input_buffer();

            new_log->food_id = selected_food.food_id;
            strcpy(new_log->food_name, selected_food.name);
            new_log->total_calories = new_log->quantity_consumed * selected_food.calories_per_unit;

            // Add to front of the list for that day
            new_log->next_diet_log = member->diet_heads[day_index];
            member->diet_heads[day_index] = new_log;

            printf("Logged %.2f %s of %s (%.2f kcal).\n", 
                new_log->quantity_consumed, selected_food.unit, new_log->food_name, new_log->total_calories);
        }

        printf("Log another food item for this day? (y/n): ");
        scanf(" %c", &add_another);
        clear_input_buffer();

    } while (add_another == 'y' || add_another == 'Y');
}

void display_diet_log(Member* member) {
    if (!member) return;
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    printf("\n--- Diet Log for %s ---\n", member->name);
    
    int diet_found = 0;
    for (int i = 0; i < num_days; i++) {
        DietLog* current = member->diet_heads[i];
        if (current != NULL) {
            diet_found = 1;
            printf("\n--- %s ---\n", days[i]);
            printf("  %-20s | Quantity | Total Calories\n", "Food Item");
            printf("  ------------------------------------------\n");
            while (current != NULL) {
                printf("  %-20s | %-8.2f | %.2f kcal\n", current->food_name, current->quantity_consumed, current->total_calories);
                current = current->next_diet_log;
            }
        }
    }
    if (!diet_found) {
        printf("No diet items logged for this member yet.\n");
    }
}

void calculate_total_calories(Member* member) {
    if (!member) {
        printf("Error: No member selected.\n");
        return;
    }

    int day_index = get_day_index();
    char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    
    float total_cals = 0;
    DietLog* current = member->diet_heads[day_index];

    while (current != NULL) {
        total_cals += current->total_calories;
        current = current->next_diet_log;
    }

    printf("\nTotal calories consumed by %s on %s: %.2f kcal\n", member->name, days[day_index], total_cals);
}