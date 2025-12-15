#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "coordinates.h"

class Block {
  public:
    Block(): positions(std::vector<Coordinates>()) {}
    Block(const std::vector<Coordinates>& pos): positions(pos) {}
    const std::vector<Coordinates>& get_positions() const;

    int get_score() const;

    Block(const Block& other) : positions(other.positions) {}

    Block& operator=(const Block& other) {
        if (this != &other) {
            const_cast<std::vector<Coordinates>&>(positions) = other.positions;
        }
        return *this;
    }
    bool operator==(const Block& other) const {
        return positions == other.positions;
    }

  private:
    std::vector<Coordinates> positions;
};

#endif // BLOCK_H