#include "draggable_figure.h"
#include <FL/Fl.H>
#include <cmath>

namespace BlockBlast {

DraggableFigure::DraggableFigure(Graph_lib::Window* w) : win(w) {}

DraggableFigure::~DraggableFigure() {
    release_blocks();
}

void DraggableFigure::add_block(Graph_lib::Point p, int size, Graph_lib::Color fill) {
    auto* r = new Graph_lib::Rectangle{p, size, size};
    r->set_fill_color(fill);
    r->set_color(Graph_lib::Color::white);
    if (blocks.empty()) {
        blocks.push_back({r, 0, 0});
        start_base_x = p.x;
        start_base_y = p.y;
    } else {
        int raw_dx = p.x - start_base_x;
        int raw_dy = p.y - start_base_y;

        int dx = static_cast<int>(std::lround(static_cast<double>(raw_dx) / size)) * size;
        int dy = static_cast<int>(std::lround(static_cast<double>(raw_dy) / size)) * size;

        blocks.push_back({r, dx, dy});
    }

    win->attach(*r);
}

void DraggableFigure::save_start() {
    if (!blocks.empty()) {
        start_base_x = blocks[0].rect->point(0).x;
        start_base_y = blocks[0].rect->point(0).y;
    }
}

void DraggableFigure::move_back_to_start() {
    if (!available || blocks.empty()) return;
    int dx = start_base_x - blocks[0].rect->point(0).x;
    int dy = start_base_y - blocks[0].rect->point(0).y;
    for (auto& b : blocks)
        b.rect->move(dx, dy);
    Fl::redraw();
}

bool DraggableFigure::contains(int x, int y) {
    if (!available) return false;
    for (auto& b : blocks) {
        Graph_lib::Point p = b.rect->point(0);
        if (x >= p.x && x < p.x + b.rect->width() &&
            y >= p.y && y < p.y + b.rect->height())
            return true;
    }
    return false;
}

void DraggableFigure::start_drag(int x, int y) {
    if (blocks.empty()) return;
    dragging = true;
    offset_x = x - blocks[0].rect->point(0).x;
    offset_y = y - blocks[0].rect->point(0).y;
}

void DraggableFigure::drag_to(int x, int y) {
    if (!dragging || blocks.empty()) return;

    int new_base_x = x - offset_x;
    int new_base_y = y - offset_y;

    int dx = new_base_x - blocks[0].rect->point(0).x;
    int dy = new_base_y - blocks[0].rect->point(0).y;

    if (dx == 0 && dy == 0) return;

    for (auto& b : blocks)
        b.rect->move(dx, dy);

    Fl::redraw();
}

void DraggableFigure::stop_drag() {
    dragging = false;
}

bool DraggableFigure::is_available() const {
    return available && !blocks.empty();
}

void DraggableFigure::consume() {
    if (!available) return;
    available = false;
    release_blocks();
}

Graph_lib::Color DraggableFigure::figure_color() const {
    if (blocks.empty()) return Graph_lib::Color::invisible;
    return blocks[0].rect->fill_color();
}

int DraggableFigure::get_block_size() const {
    return blocks.size();
}

std::vector<std::pair<int, int>> DraggableFigure::get_relative_positions() const {
    std::vector<std::pair<int, int>> positions;
    for (const auto& b : blocks) {

        constexpr int CELL = 40;
        positions.push_back({b.dx / CELL, b.dy / CELL});
    }
    return positions;
}

void DraggableFigure::release_blocks() {
    for (auto& b : blocks) {
        if (b.rect) {
            win->detach(*b.rect);
            delete b.rect;
            b.rect = nullptr;
        }
    }
    blocks.clear();
}

} // namespace BlockBlast