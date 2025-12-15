#include "game.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>

Game::Game(unsigned int width, unsigned int height) :
    x_size(width),
    y_size(height),
    game_field(width, height),
    score(0) {
}

void Game::initialize_types_of_blocks() {
    types_of_blocks.clear();

    // Блок 1: вертикальная линия (4 клетки)
    std::vector<Coordinates> block1_coords;
    block1_coords.push_back(Coordinates(0, 0));
    block1_coords.push_back(Coordinates(0, 1));
    block1_coords.push_back(Coordinates(0, 2));
    block1_coords.push_back(Coordinates(0, 3));
    types_of_blocks.push_back(Block(block1_coords));

    // Блок 2: горизонтальная линия (4 клетки)
    std::vector<Coordinates> block2_coords;
    block2_coords.push_back(Coordinates(0, 0));
    block2_coords.push_back(Coordinates(1, 0));
    block2_coords.push_back(Coordinates(2, 0));
    block2_coords.push_back(Coordinates(3, 0));
    types_of_blocks.push_back(Block(block2_coords));

    // Блок 3: квадрат (2x2)
    std::vector<Coordinates> block3_coords;
    block3_coords.push_back(Coordinates(0, 0));
    block3_coords.push_back(Coordinates(1, 0));
    block3_coords.push_back(Coordinates(0, 1));
    block3_coords.push_back(Coordinates(1, 1));
    types_of_blocks.push_back(Block(block3_coords));

    // Блок 4: маленький L
    std::vector<Coordinates> block4_coords;
    block4_coords.push_back(Coordinates(0, 0));
    block4_coords.push_back(Coordinates(1, 0));
    block4_coords.push_back(Coordinates(0, 1));
    types_of_blocks.push_back(Block(block4_coords));

    // Блок 5: большой L
    std::vector<Coordinates> block5_coords;
    block5_coords.push_back(Coordinates(0, 0));
    block5_coords.push_back(Coordinates(0, 1));
    block5_coords.push_back(Coordinates(0, 2));
    block5_coords.push_back(Coordinates(1, 0));
    block5_coords.push_back(Coordinates(2, 0));
    types_of_blocks.push_back(Block(block5_coords));

    // Блок 6: диагональ маленькая
    std::vector<Coordinates> block6_coords;
    block6_coords.push_back(Coordinates(0, 0));
    block6_coords.push_back(Coordinates(0, 1));
    block6_coords.push_back(Coordinates(1, 1));
    types_of_blocks.push_back(Block(block6_coords));

    // Блок 7: диагональ маленькая отраженная
    std::vector<Coordinates> block7_coords;
    block7_coords.push_back(Coordinates(0, 0));
    block7_coords.push_back(Coordinates(1, 0));
    block7_coords.push_back(Coordinates(1, 1));
    types_of_blocks.push_back(Block(block7_coords));

    // Блок 8: пирамидка
    std::vector<Coordinates> block8_coords;
    block8_coords.push_back(Coordinates(0, 0));
    block8_coords.push_back(Coordinates(1, 0));
    block8_coords.push_back(Coordinates(2, 0));
    block8_coords.push_back(Coordinates(1, 1));
    types_of_blocks.push_back(Block(block8_coords));

    // Блок 9: квадратик(1x1)
    std::vector<Coordinates> block9_coords;
    block9_coords.push_back(Coordinates(1, 0));
    types_of_blocks.push_back(Block(block9_coords));

    // Блок 10: полоска горизонтальная(1x2)
    std::vector<Coordinates> block10_coords;
    block10_coords.push_back(Coordinates(0, 0));
    block10_coords.push_back(Coordinates(1, 0));
    types_of_blocks.push_back(Block(block10_coords));

    // Блок 11: полоска вертикальная(1x2)
    std::vector<Coordinates> block11_coords;
    block11_coords.push_back(Coordinates(0, 0));
    block11_coords.push_back(Coordinates(0, 1));
    types_of_blocks.push_back(Block(block11_coords));

    // Блок 12: пирамидка перевернутая
    std::vector<Coordinates> block12_coords;
    block12_coords.push_back(Coordinates(0, 0));
    block12_coords.push_back(Coordinates(1, 0));
    block12_coords.push_back(Coordinates(2, 0));
    block12_coords.push_back(Coordinates(1, -1));
    types_of_blocks.push_back(Block(block12_coords));

    // Блок 13: квадрат (3x3)
    std::vector<Coordinates> block13_coords;
    block13_coords.push_back(Coordinates(0, 0));
    block13_coords.push_back(Coordinates(1, 0));
    block13_coords.push_back(Coordinates(0, 1));
    block13_coords.push_back(Coordinates(1, 1));
    block13_coords.push_back(Coordinates(2, 0));
    block13_coords.push_back(Coordinates(2, 1));
    block13_coords.push_back(Coordinates(0, 2));
    block13_coords.push_back(Coordinates(1, 2));
    block13_coords.push_back(Coordinates(2, 2));
    types_of_blocks.push_back(Block(block13_coords));

    // Блок 14: пирамидка горизонтальная
    std::vector<Coordinates> block14_coords;
    block14_coords.push_back(Coordinates(0, 0));
    block14_coords.push_back(Coordinates(0, 1));
    block14_coords.push_back(Coordinates(0, 2));
    block14_coords.push_back(Coordinates(1, 1));
    types_of_blocks.push_back(Block(block14_coords));

    // Блок 15: пирамидка горизонтальная(перевернутая)
    std::vector<Coordinates> block15_coords;
    block15_coords.push_back(Coordinates(0, 0));
    block15_coords.push_back(Coordinates(0, 1));
    block15_coords.push_back(Coordinates(0, 2));
    block15_coords.push_back(Coordinates(-1, 1));
    types_of_blocks.push_back(Block(block15_coords));

    // Блок 16: полоска вертикальная(1x3)
    std::vector<Coordinates> block16_coords;
    block16_coords.push_back(Coordinates(0, 0));
    block16_coords.push_back(Coordinates(0, 1));
    block16_coords.push_back(Coordinates(0, 2));
    types_of_blocks.push_back(Block(block16_coords));

    // Блок 17: полоска горизонтальная(1x3)
    std::vector<Coordinates> block17_coords;
    block17_coords.push_back(Coordinates(0, 0));
    block17_coords.push_back(Coordinates(1, 0));
    types_of_blocks.push_back(Block(block17_coords));

    // Блок 18: Буква "Г"
    std::vector<Coordinates> block18_coords;
    block18_coords.push_back(Coordinates(0, 0));
    block18_coords.push_back(Coordinates(1, 0));
    block18_coords.push_back(Coordinates(2, 0));
    block18_coords.push_back(Coordinates(2, 1));
    types_of_blocks.push_back(Block(block18_coords));

    // Блок 19: "Зиг-заг"
    std::vector<Coordinates> block19_coords;
    block19_coords.push_back(Coordinates(0, 0));
    block19_coords.push_back(Coordinates(1, 0));
    block19_coords.push_back(Coordinates(1, 1));
    block19_coords.push_back(Coordinates(2, 1));
    types_of_blocks.push_back(Block(block19_coords));

}

Block Game::get_random_block() {
    if (types_of_blocks.empty()) {
        std::cerr << "No block types loaded!" << std::endl;
        return Block();
    }
    int number = rand() % types_of_blocks.size();
    return types_of_blocks[number];
}

std::vector<Block> Game::generate_blocks() {
    std::vector<Block> blocks;
    for (int i = 0 ; i < COUNT_OF_BLOCKS; i++) {
        blocks.push_back(get_random_block());
    }
    return blocks;
}

bool Game::is_game_over() {
    // Проверяем, есть ли хотя бы один блок, который можно разместить
    for (const auto& block : types_of_blocks) {
        for (unsigned int x = 0; x < x_size; x++) {
            for (unsigned int y = 0; y < y_size; y++) {
                Coordinates pos(static_cast<int>(x), static_cast<int>(y));
                if (game_field.is_placebale(block, pos)) {
                    return false; // Есть хотя бы одно возможное размещение
                }
            }
        }
    }
    std::cout << "Game Over! Final Score: " << score << std::endl;
    return true;
}

bool Game::make_step() {
    current_blocks = generate_blocks();
    if (is_game_over())
        return false;

    for (const auto& block: current_blocks) {
        bool placed = false;
        for (unsigned int x = 0; x < x_size && !placed; x++) {
            for (unsigned int y = 0; y < y_size && !placed; y++) {
                Coordinates pos(static_cast<int>(x), static_cast<int>(y));
                if (game_field.is_placebale(block, pos)) {
                    game_field.place(block, pos);
                    // Обновляем очки после размещения
                    int points = game_field.update(); // Вызываем update вручную
                    score += points;
                    if (points > 0) {
                        std::cout << "Scored " << points << " points! Total: " << score << std::endl;
                    }
                    placed = true;
                }
            }
        }
        if (!placed) {
            std::cout << "Could not place a block!" << std::endl;
        }
    }

    current_blocks.clear();

    return !is_game_over();
}

void Game::get_hint(Block& best_block, Coordinates& best_coords) {
    best_coords = Coordinates(-1, -1); // не найден ход
    int best_perimeter = 0;
    for (const auto& cur_block: current_blocks) {
        for (int x = 0; x < x_size; x++) {
            for (int y = 0; y < y_size; y++) {
                Coordinates pos(x, y);
                if (game_field.is_placebale(cur_block, pos)) {
                      if (best_coords.x == -1) {
                          best_coords = pos;
                          best_block = cur_block;
                      }

                      Game other = *this;
                      other.game_field.place(cur_block, pos);
                      if (other.is_game_over()) continue;

                      int perimeter = 0;
                      for (const auto& cords: cur_block.get_positions()) {
                          int x_ceil = cords.x + pos.x;
                          int y_ceil = cords.y + pos.y;
                          if (x_ceil + 1 < x_size && !game_field.is_free(Coordinates(x_ceil + 1, y_ceil)))
                              perimeter += 1;
                          if (x_ceil - 1 >= 0 && !game_field.is_free(Coordinates(x_ceil - 1, y_ceil)))
                              perimeter += 1;
                          if (y_ceil + 1 < y_size && !game_field.is_free(Coordinates(y_ceil + 1, x_ceil)))
                              perimeter += 1;
                          if (y_ceil - 1 >= 0 && !game_field.is_free(Coordinates(y_ceil - 1, x_ceil)))
                              perimeter += 1;
                          if (perimeter > best_perimeter) {
                              best_perimeter = perimeter;
                              best_block = cur_block;
                              best_coords = pos;
                          }
                      }
                }
            }
        }
    }
}