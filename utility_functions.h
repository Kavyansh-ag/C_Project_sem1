#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include "types.h"
#include "constants.h"

/* ---------- UTILITY / HELPER FUNCTIONS ---------- */

float calculate_bmi(float height, float weight);

void clear_input_buffer();

// int get_valid_int_input(const char* prompt);

// float get_valid_float_input(const char* prompt);

// void press_enter_to_continue();

int get_day_index();

int get_body_part_index();

#endif
