#include "grid_board.h"
#include "constants.h"
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <cmath>
#include <iostream>

namespace BlockBlast {

GridBoard::GridBoard(Graph_lib::Window* w, ::Field& field) :
    game_field(&field), win(w) {}

void GridBoard::draw_board() {
    const int bg_x = OFFSET_X - BACKGROUND_PADDING;
    const int bg_y = OFFSET_Y - BACKGROUND_PADDING;
    const int bg_size = SIZE * CELL + BACKGROUND_PADDING * 2;
    backdrop = new Graph_lib::Rectangle{Graph_lib::Point{bg_x, bg_y}, bg_size, bg_size};
    backdrop->set_color(Graph_lib::Color{static_cast<int>(fl_rgb_color(18, 34, 62))});
    backdrop->set_fill_color(Graph_lib::Color::invisible);
    win->attach(*backdrop);

    const int strip_height = 6;
    for (int y = 0; y < bg_size; y += strip_height) {
        const double ratio = static_cast<double>(y) / bg_size;
        const int r = 24 + static_cast<int>((18 - 24) * ratio);
        const int g = 56 + static_cast<int>((34 - 56) * ratio);
        const int b = 92 + static_cast<int>((62 - 92) * ratio);
        fl_color(r, g, b);
        fl_rectf(bg_x, bg_y + y, bg_size, strip_height);
    }

    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            int x = OFFSET_X + c * CELL;
            int y = OFFSET_Y + r * CELL;
            auto* rect = new Graph_lib::Rectangle{Graph_lib::Point{x, y}, CELL, CELL};
            const double row_ratio = static_cast<double>(r) / (SIZE - 1);
            const double col_ratio = static_cast<double>(c) / (SIZE - 1);
            const int red = 140 + static_cast<int>(60 * col_ratio);
            const int green = 200 + static_cast<int>(40 * row_ratio);
            const int blue = 220 + static_cast<int>(25 * row_ratio);
            const int border_color = static_cast<int>(fl_rgb_color(90, 140, 185));
            const int cell_fill = static_cast<int>(fl_rgb_color(red, green, blue));
            rect->set_color(Graph_lib::Color{border_color});
            rect->set_fill_color(Graph_lib::Color{cell_fill});
            win->attach(*rect);
            cells.push_back(rect);
        }
    }
}

bool GridBoard::cell_empty(int r, int c) const {
    Coordinates coord(c, r);
    return game_field->is_free(coord);
}

bool GridBoard::pixel_to_cell_strict(int x, int y, int& r, int& c) const {
    if (x < OFFSET_X || y < OFFSET_Y) return false;
    c = (x - OFFSET_X) / CELL;
    r = (y - OFFSET_Y) / CELL;
    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) return false;
    return true;
}

void GridBoard::pixel_to_cell_nearest(int x, int y, int& r, int& c) const {
    double fx = double(x - OFFSET_X) / CELL;
    double fy = double(y - OFFSET_Y) / CELL;
    c = static_cast<int>(std::lround(fx));
    r = static_cast<int>(std::lround(fy));
}

bool GridBoard::can_place(DraggableFigure& fig) {
    if (fig.blocks.empty()) return false;

    Graph_lib::Point base_px = fig.blocks[0].rect->point(0);

    int base_r, base_c;
    pixel_to_cell_nearest(base_px.x, base_px.y, base_r, base_c);

    for (auto& b : fig.blocks) {
        int dc = b.dx / CELL;
        int dr = b.dy / CELL;
        int r = base_r + dr;
        int c = base_c + dc;

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) return false;

        Coordinates coord(c, r);
        if (!game_field->is_free(coord)) return false;
    }
    return true;
}

int GridBoard::place(DraggableFigure& fig) {
    if (fig.blocks.empty()) return 0;

    Graph_lib::Point base_px = fig.blocks[0].rect->point(0);

    int base_r, base_c;
    pixel_to_cell_nearest(base_px.x, base_px.y, base_r, base_c);

    std::vector<Coordinates> relative_positions;
    for (auto& b : fig.blocks) {
        int dc = b.dx / CELL;
        int dr = b.dy / CELL;
        int r = base_r + dr;
        int c = base_c + dc;

        if (r < 0 || r >= SIZE || c < 0 || c >= SIZE) continue;

        relative_positions.push_back(Coordinates(dc, dr));
    }

    if (!relative_positions.empty()) {
        Block temp_block(relative_positions);
        Coordinates left_bottom(base_c, base_r);

        if (game_field->is_placebale(temp_block, left_bottom)) {
            int points = game_field->place(temp_block, left_bottom);
            update_display();
            return points;
        }
    }
    return 0;
}

void GridBoard::update_display() {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            Coordinates coord(c, r);
            int index = r * SIZE + c;

            if (game_field->is_free(coord)) {
                const double row_ratio = static_cast<double>(r) / (SIZE - 1);
                const double col_ratio = static_cast<double>(c) / (SIZE - 1);
                const int red = 140 + static_cast<int>(60 * col_ratio);
                const int green = 200 + static_cast<int>(40 * row_ratio);
                const int blue = 220 + static_cast<int>(25 * row_ratio);
                cells[index]->set_fill_color(Graph_lib::Color(red, green, blue));
                cells[index]->set_color(Graph_lib::Color(90, 140, 185));
            } else {
                cells[index]->set_fill_color(Graph_lib::Color::dark_red);
                cells[index]->set_color(Graph_lib::Color::black);
            }
        }
    }
    Fl::redraw();
}

bool GridBoard::has_available_move(DraggableFigure& fig) {
    auto relative_positions = fig.get_relative_positions();

    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            bool can_place_here = true;

            for (const auto& rel : relative_positions) {
                int cell_r = r + rel.second;
                int cell_c = c + rel.first;

                if (cell_r < 0 || cell_r >= SIZE || cell_c < 0 || cell_c >= SIZE) {
                    can_place_here = false;
                    break;
                }

                Coordinates coord(cell_c, cell_r);
                if (!game_field->is_free(coord)) {
                    can_place_here = false;
                    break;
                }
            }

            if (can_place_here) {
                return true;
            }
        }
    }

    return false;
}

} // namespace BlockBlast