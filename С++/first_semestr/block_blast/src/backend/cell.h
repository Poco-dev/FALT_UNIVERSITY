#ifndef CELL_H
#define CELL_H

#include "coordinates.h"

enum class Cell_type {
  free,
  filled
};

class Cell {
 public:
  Cell(): type(Cell_type::free), coord(0, 0) {}
  Cell(Cell_type type, Coordinates cords) : type(type),  coord(cords){}

  bool is_free();
  void change_type(Cell_type new_type);

 private:
    Cell_type type;
    Coordinates coord;
};
#endif //CELL_H
