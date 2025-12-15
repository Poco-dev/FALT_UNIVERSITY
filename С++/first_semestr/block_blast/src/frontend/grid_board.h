#ifndef GRID_BOARD_GUARD
#define GRID_BOARD_GUARD

#include <vector>
#include "Graph_lib/Window.h"
#include "Graph_lib/Graph.h"
#include "../backend/game.h"
#include "draggable_figure.h"
#include "constants.h"

namespace BlockBlast {

struct GridBoard {
    static constexpr int SIZE = GRID_SIZE;
    static constexpr int CELL = ::BlockBlast::CELL;
    static constexpr int OFFSET_X = 300;
    static constexpr int OFFSET_Y = 40;
    static constexpr int BACKGROUND_PADDING = 24;

    ::Field* game_field;
    Graph_lib::Window* win;
    std::vector<Graph_lib::Rectangle*> cells;
    Graph_lib::Rectangle* backdrop = nullptr;

    GridBoard(Graph_lib::Window* w, ::Field& field);

    void draw_board();
    bool cell_empty(int r, int c) const;
    bool pixel_to_cell_strict(int x, int y, int& r, int& c) const;
    void pixel_to_cell_nearest(int x, int y, int& r, int& c) const;
    bool can_place(DraggableFigure& fig);
    int place(DraggableFigure& fig);
    void update_display();
    bool has_available_move(DraggableFigure& fig);
};

} // namespace BlockBlast

#endif