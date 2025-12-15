#include "Graph_lib/Window.h"
#include "Graph_lib/GUI.h"

namespace Graph_lib {

void Window::init()
{
    resizable(this);
    show();
}

Window::Window(int w, int h, const std::string& title)
    : Fl_Double_Window{w, h, title.c_str()}, width_{w}, height_{h}
{
    init();
}

Window::Window(Point xy, int w, int h, const std::string& title)
    : Fl_Double_Window{xy.x, xy.y, w, h, title.c_str()}, width_{w}, height_{h}
{
    init();
}

void Window::attach(Shape& s)
{
    shapes.push_back(&s);
}

void Window::attach(Widget& w)
{
    begin();
    w.attach(*this);
    end();
}

void Window::detach(Shape& s)
{
    for (unsigned int i = shapes.size(); 0 < i; --i) {
        if (shapes[i-1] == &s) {
            shapes.erase(shapes.begin() + (i-1));
        }
    }
}

void Window::detach(Widget& w)
{
    w.hide();
}

void Window::put_on_top(Shape& s)
{
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        if (&s == shapes[i]) {
            if (i == shapes.size() - 1) {
                return;
            }
            for (++i; i < shapes.size(); ++i) {
                shapes[i-1] = shapes[i];
            }
            shapes[shapes.size() - 1] = &s;
            return;
        }
    }
}

int gui_main()
{
    return Fl::run();
}

} // namespace Graph_lib