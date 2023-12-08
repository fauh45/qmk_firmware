#include "matrix.h"
#include "quantum.h"

#define COL_SHIFTER ((uint16_t)1)

static bool colABpressed = false;
// static bool encoderPressed = false;

void clicked(void) {
    tap_code(KC_MPLY);
}

void turned(bool clockwise) {
    if (IS_LAYER_ON(4)) {
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    } else if (IS_LAYER_ON(1)) {
        tap_code16(clockwise ? LCTL(KC_TAB) : LCTL(LSFT(KC_TAB)));
    } else if (IS_LAYER_ON(2)) {
        tap_code(clockwise ? KC_MS_WH_DOWN : KC_MS_WH_UP);
    } else {
        tap_code16(clockwise ? LGUI(KC_Y) : LGUI(KC_Z));
    }
}

void blank_column(matrix_row_t current_matrix[], uint8_t col) {
    uint16_t column_index_bitmask = COL_SHIFTER << col;
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS-1; row_index++) {
        current_matrix[row_index] &= ~column_index_bitmask;
    }
}

bool is_entire_column_held(matrix_row_t current_matrix[], uint8_t col) {
    uint16_t column_index_bitmask = COL_SHIFTER << col;
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS-1; row_index++) {
        if (!(current_matrix[row_index] & column_index_bitmask)) return false;
    }
    return true;
}

// Because of a bug in the routing of the cheapino, encoder action
// triggers entire columns... fix it in software here, assumption is that
// you never press an entire column, sounds safe?
void fix_encoder_action(matrix_row_t current_matrix[]) {

    // Disabled for testing
    // 7th column means encoder was pressed
    // if (is_entire_column_held(current_matrix, 7)) {
    //     encoderPressed = true;
    //     blank_column(current_matrix, 7);
    //     current_matrix[3] |= COL_SHIFTER;
    // } else {
    //     current_matrix[3] &= ~COL_SHIFTER;
        // Only trigger click on release
    //     if (encoderPressed) {
    //         encoderPressed = false;
    //         clicked();
    //     }
    // }

    // Now check rotary action:
    // bool colA = is_entire_column_held(current_matrix, 8);
    // bool colB = is_entire_column_held(current_matrix, 10);

    // matrix_row_t row4 = current_matrix[7];

    // Check if col2 is "pressed"
    bool colA = row4 & COL_SHIFTER << 8;
    // Check if col3 is "pressed"
    bool colB = row4 & COL_SHIFTER << 10;

    if (colA && colB) {
        colABpressed = true;
        // blank_column(current_matrix, 8);
        // blank_column(current_matrix, 10);
    } else if (colA) {
        if (colABpressed) {
            colABpressed = false;
            turned(true);
        }
        // blank_column(current_matrix, 8);
    } else if (colB) {
        if (colABpressed) {
            colABpressed = false;
            turned(false);
        }
        // blank_column(current_matrix, 10);
    }
}
