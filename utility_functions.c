#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "member.h"
#include "utility_functions.h"
#include "types.h"
#include "constants.h"




float calculate_bmi(float height, float weight)
{   
     if (height <= 0)
        return 0; // invalid i/p  
    return weight/(height*height); 
}


/* This reads and discards characters from the input stream
    until it hits a newline or the end of the file */
void clear_input_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_day_index() {
    int choice;
    do {
        printf("\nSelect the day of the week:\n");
        printf("  0: Sunday\n");
        printf("  1: Monday\n");
        printf("  2: Tuesday\n");
        printf("  3: Wednesday\n");
        printf("  4: Thursday\n");
        printf("  5: Friday\n");
        printf("  6: Saturday\n");
        printf("Enter your choice (0-6): ");
        if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter digits only.\n");
        clear_input_buffer();
        choice = -1;   // Force invalid condition
        continue;
}
clear_input_buffer();


        if (choice < 0 || choice > 6) {
            printf("Invalid choice. Please enter a number between 0 and 6.\n");
        }
    } while (choice < 0 || choice > 6);
    return choice;
}

int get_body_part_index() {
    int choice = -1;
    do {
        printf("\nSelect the body part you focused on:\n");
        printf("  0: Chest\n");
        printf("  1: Back\n");
        printf("  2: Legs\n");
        printf("  3: Arms\n");
        printf("  4: Shoulders\n");
        printf("Enter your choice (0-4): ");
        if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter numbers only.\n");
        clear_input_buffer();
        choice = -1;
        continue;
}
clear_input_buffer();


        if (choice < 0 || choice > 4) {
            printf("Invalid choice. Please enter a number between 0 and 4.\n");
        }
    } while (choice < 0 || choice > 4);
    return choice;
}