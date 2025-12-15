#ifndef GAME_H
#define GAME_H

#include "field.h"
#include "block.h"
#include <string>
#include <vector>

const int COUNT_OF_BLOCKS = 3;

class Game {
 public:
   void initialize_types_of_blocks();
    Game(unsigned int width, unsigned int height);
    bool make_step();
    std::vector<Block> generate_blocks();
    Block get_random_block();
    bool is_game_over();

    unsigned int get_score() const { return score; }

    // Геттер для Field
    void get_hint(Block& best_block, Coordinates& best_coords);
    Field& get_field() { return game_field; }
    const Field& get_field() const { return game_field; }
    void update_score() {
        score = static_cast<unsigned int>(game_field.get_current_score());
    }

 private:
    unsigned int x_size, y_size;
    std::vector<Block> types_of_blocks;
    std::vector<Block> current_blocks;
    Field game_field;
    unsigned int score;
};

#endif //GAME_H