#include "block.h"

const std::vector<Coordinates>& Block::get_positions() const {
  return positions;
}

int Block::get_score() const{
    return positions.size();
}