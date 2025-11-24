#include "flags.h"

void flags_add(unsigned int *value, unsigned int flags_to_add) {
    *value |= flags_to_add;
}

void flags_remove(unsigned int *value, unsigned int flags_to_remove) {
    *value &= ~flags_to_remove;
}

void flags_toggle(unsigned int *value, unsigned int flags_to_toggle) {
    *value ^= flags_to_toggle;
}

bool flags_has_all(unsigned int value, unsigned int flags_to_check) {
    return (value & flags_to_check) == flags_to_check;
}

bool flags_has_any(unsigned int value, unsigned int flags_to_check) {
    return (value & flags_to_check) != 0;
}