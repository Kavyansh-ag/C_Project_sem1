#ifndef FILE_IO_H
#define FILE_IO_H

#include "types.h"
#include "constants.h"   // for max_menu_items, num_days, etc.


/* -------- FILE I/O FUNCTION PROTOTYPES -------- */

void save_data(Member* head);

Member* load_data(
    ExerciseItem exercise_db[num_focus_areas][max_exercise_per_category],
    FoodItem daily_menus[num_days][max_menu_items]
);

#endif
