#include "cell.h"

bool Cell::is_free() {
  return type == Cell_type::free;
}

void Cell::change_type(Cell_type new_type) {
    type = new_type;
}