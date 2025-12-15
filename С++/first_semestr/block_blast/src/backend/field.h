#ifndef FIELD_H
#define FIELD_H

#include <vector>

#include "cell.h"
#include "block.h"
#include "coordinates.h"

class Field {
 public:
    Field(unsigned int x, unsigned int y);
    void clear();
    bool is_placebale(const Block& block, Coordinates left_bottom);
    int update();
    int place(const Block& block, Coordinates left_bottom);
    bool is_free(Coordinates coordinates);
    int get_current_score() const { return current_score; }

 private:
   const unsigned int x_size;
   const unsigned int y_size;
   std::vector<std::vector<Cell> > field;
   int current_score = 0;
};

#endif //FIELD_H