#ifndef SIMPLE_WINDOW_GUARD
#define SIMPLE_WINDOW_GUARD 1

#include "GUI.h"
#include "Window.h"

namespace Graph_lib {

struct Simple_window : Window {
    Simple_window(Point xy, int w, int h, const std::string& title)
        : Window{xy, w, h, title},
          next_button{Point{x_max() - 70, 0}, 70, 20, "Next", cb_next}
    {
        attach(next_button);
    }

    void wait_for_button() {
        while (!button_pushed && Fl::wait());
        button_pushed = false;
        Fl::redraw();
    }

    Button next_button;

private:
    bool button_pushed = false;

    static void cb_next(Address, Address addr)
    {
        auto* pb = static_cast<Button*>(addr);
        static_cast<Simple_window&>(pb->window()).next();
    }

    void next() { button_pushed = true; }
};

} // namespace Graph_lib

#endif