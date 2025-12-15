#ifndef WINDOW_GUARD
#define WINDOW_GUARD 1

#include <string>
#include <vector>

#include "Point.h"
#include "Graph.h"
#include "fltk.h"

namespace Graph_lib {

class Shape;
class Widget;

class Window : public Fl_Double_Window
{
public:
    using Fl_Window::resize;

    Window(int w, int h, const std::string& title);
    Window(Point xy, int w, int h, const std::string& title);

    virtual ~Window() {}

    int x_max() const { return width_; }
    int y_max() const { return height_; }

    void resize(int ww, int hh)
    {
        width_ = ww;
        height_ = hh;
        size(ww, hh);
    }

    void set_label(const std::string& s) { label(s.c_str()); }

    void attach(Shape& s);
    void attach(Widget& w);

    void detach(Shape& s);
    void detach(Widget& w);

    void put_on_top(Shape& s);

protected:
    void draw() override {
        Fl_Window::draw();
        // Затем рисуем пользовательский фон поверх стандартного
        draw_background();
        // И только потом прикреплённые фигуры
        for (auto* sp : shapes) {
            if (sp) sp->draw();
        }
    }

    virtual void draw_background() {
        fl_color(FL_WHITE);
        fl_rectf(0, 0, width_, height_);
    }

private:
    std::vector<Shape*> shapes;
    int width_;
    int height_;

    void init();
};

int gui_main();

inline int x_max() { return Fl::w(); }
inline int y_max() { return Fl::h(); }

} // namespace Graph_lib

#endif