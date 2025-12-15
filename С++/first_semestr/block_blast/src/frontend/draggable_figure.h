#ifndef DRAGGABLE_FIGURE_GUARD
#define DRAGGABLE_FIGURE_GUARD

#include <vector>
#include <memory>
#include <utility>
#include "Graph_lib/Window.h"
#include "Graph_lib/Graph.h"
#include "constants.h"

namespace BlockBlast {

struct DraggableFigure {
    struct BlockInfo {
        Graph_lib::Rectangle* rect;
        int dx;
        int dy;
    };

    std::vector<BlockInfo> blocks;
    bool dragging = false;
    bool available = true;
    int offset_x{};
    int offset_y{};
    int start_base_x{};
    int start_base_y{};
    Graph_lib::Window* win;

    DraggableFigure(Graph_lib::Window* w);
    ~DraggableFigure();

    void add_block(Graph_lib::Point p, int size, Graph_lib::Color fill);
    void save_start();
    void move_back_to_start();
    bool contains(int x, int y);
    void start_drag(int x, int y);
    void drag_to(int x, int y);
    void stop_drag();
    bool is_available() const;
    void consume();
    Graph_lib::Color figure_color() const;
    int get_block_size() const;
    std::vector<std::pair<int, int>> get_relative_positions() const;

private:
    void release_blocks();
};

} // namespace BlockBlast

#endif