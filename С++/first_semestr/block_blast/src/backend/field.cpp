#include "field.h"

Field::Field(unsigned int x, unsigned int y): x_size(x), y_size(y), current_score(0) {
    field = std::vector<std::vector<Cell> >(x_size, std::vector<Cell>(y_size, Cell()));
    for (unsigned int i = 0; i < x_size; ++i) {
        for (unsigned int j = 0; j < y_size; ++j) {
            field[i][j] = Cell(Cell_type::free, Coordinates(static_cast<int>(i), static_cast<int>(j)));
        }
    }
}

void Field::clear() {
    for (unsigned int i = 0; i < x_size; ++i) {
        for (unsigned int j = 0; j < y_size; ++j) {
            field[i][j].change_type(Cell_type::free);
        }
    }
    current_score = 0;
}

bool Field::is_free(Coordinates coordinates) {
    if (coordinates.x < 0 || coordinates.x >= static_cast<int>(x_size) ||
        coordinates.y < 0 || coordinates.y >= static_cast<int>(y_size)) {
        return false;
    }
    return field[coordinates.x][coordinates.y].is_free();
}

bool Field::is_placebale(const Block& block, Coordinates left_bottom) {
    const std::vector<Coordinates>& positions = block.get_positions();
    for (const Coordinates& pos: positions) {
        Coordinates cur_pos = left_bottom + pos;
        int cur_x = cur_pos.x;
        int cur_y = cur_pos.y;

        if (cur_x >= static_cast<int>(x_size) || cur_y >= static_cast<int>(y_size) || cur_x < 0 || cur_y < 0)
            return false;

        if (!field[cur_x][cur_y].is_free())
            return false;
    }
    return true;
}

int Field::update() {
    std::vector<unsigned int> rows;
    std::vector<unsigned int> cols;
    int points = 0;

    for (unsigned int i = 0; i < x_size; ++i) {
        bool all_filled = true;
        for (unsigned int j = 0; j < y_size; ++j) {
            if (field[i][j].is_free()) {
                all_filled = false;
                break;
            }
        }
        if (all_filled) {
            rows.push_back(i);
            points += 100;
        }
    }

    for (unsigned int j = 0; j < y_size; ++j) {
        bool all_filled = true;
        for (unsigned int i = 0; i < x_size; ++i) {
            if (field[i][j].is_free()) {
                all_filled = false;
                break;
            }
        }
        if (all_filled) {
            cols.push_back(j);
            points += 100;
        }
    }

    if (!rows.empty() && !cols.empty()) {
        points += 50 * rows.size() * cols.size();
    }

    for (unsigned int row : rows) {
        for (unsigned int y = 0; y < y_size; ++y) {
            field[row][y].change_type(Cell_type::free);
        }
    }

    for (unsigned int col : cols) {
        for (unsigned int x = 0; x < x_size; ++x) {
            field[x][col].change_type(Cell_type::free);
        }
    }

    current_score += points;

    return points;
}

int Field::place(const Block& block, Coordinates left_bottom) {
    const std::vector<Coordinates>& positions = block.get_positions();

    int placement_points = positions.size() * 10;

    for (const Coordinates& pos: positions) {
        Coordinates cur_pos = left_bottom + pos;
        if (cur_pos.x >= 0 && cur_pos.x < static_cast<int>(x_size) &&
            cur_pos.y >= 0 && cur_pos.y < static_cast<int>(y_size)) {
            field[cur_pos.x][cur_pos.y].change_type(Cell_type::filled);
        }
    }
    int clear_points = update();
    return placement_points + clear_points;
}