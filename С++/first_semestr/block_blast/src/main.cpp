#include "frontend/game_window.h"
#include <iostream>
#include <exception>

int main() {
    try {
        BlockBlast::GameWindow win(Graph_lib::Point(250, 100), 800, 600);
        return Graph_lib::gui_main();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}