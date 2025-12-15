#include "game_window.h"
#include "draggable_figure.h"
#include "grid_board.h"
#include "../backend/game.h"
#include "constants.h"
#include <FL/Fl.H>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace BlockBlast {

GameWindow::GameWindow(Graph_lib::Point p, int w, int h, const std::string& title) :
    Graph_lib::Window(p, w, h, title),
    board(this, generator.get_field()),
    generator(GRID_SIZE, GRID_SIZE),
    spawn_positions{
        Graph_lib::Point{60, 70},
        Graph_lib::Point{60, 230},
        Graph_lib::Point{60, 390}
    },
    color_palette{
    Graph_lib::Color(255, 99, 132),      // Розовый
    Graph_lib::Color(255, 198, 93),      // Оранжевый
    Graph_lib::Color(75, 192, 192),      // Бирюзовый
    Graph_lib::Color(54, 162, 235),      // Синий
    Graph_lib::Color(153, 102, 255),     // Фиолетовый
    Graph_lib::Color(255, 159, 64)       // Светло-оранжевый
    },
    current_score(0),
    elapsed_seconds(0),
    hints_remaining(3),
    hint_used_this_turn(false),
    record_beaten(false),
    player_name(""),
    editing_name(false)
{
    resizable(nullptr);
    size_range(w, h, w, h);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    generator.initialize_types_of_blocks();
    board.draw_board();

    record_score = get_record();

    std::string record_name = "No record";
    std::string record_time = "00:00";

    std::ifstream hs_file("highscores.txt");
    if (hs_file) {
        std::string line;
        int highest_score = 0;
        std::string best_name, best_time;

        while (std::getline(hs_file, line)) {
            std::istringstream iss(line);
            std::string name, score_str, time_str;

            if (std::getline(iss, name, ';') &&
                std::getline(iss, score_str, ';') &&
                std::getline(iss, time_str)) {

                try {
                    int score = std::stoi(score_str);
                    if (score > highest_score) {
                        highest_score = score;
                        best_name = name;
                        best_time = time_str;
                    }
                } catch (...) {
                }
            }
        }

        if (highest_score > 0) {
            record_name = best_name;
            record_time = best_time;
            record_score = highest_score;
        }
    }

    int record_x = 550;
    int record_y = 575;

    record_name_text = new Graph_lib::Text(Graph_lib::Point(record_x, record_y),
                                          "Player: " + record_name);

    record_score_text = new Graph_lib::Text(Graph_lib::Point(record_x, record_y - 50),
                                           "Score: " + std::to_string(record_score));
    record_time_text = new Graph_lib::Text(Graph_lib::Point(record_x, record_y - 100),
                                          "Time: " + record_time);


    score_text = new Graph_lib::Text(Graph_lib::Point(300, 500), "Score: 0");

    timer_text = new Graph_lib::Text(Graph_lib::Point(300, 550), "Time: 00:00");

    hints_text = new Graph_lib::Text(Graph_lib::Point(900, 500), "Hints: 3");

    score_text->set_color(Graph_lib::Color::white);
    score_text->set_font_size(30);

    record_name_text->set_color(Graph_lib::Color::white);
    record_name_text->set_font_size(24);

    record_score_text->set_color(Graph_lib::Color::white);
    record_score_text->set_font_size(24);

    record_time_text->set_color(Graph_lib::Color::white);
    record_time_text->set_font_size(24);

    timer_text->set_color(Graph_lib::Color::white);
    timer_text->set_font_size(30);

    hints_text->set_color(Graph_lib::Color::white);
    hints_text->set_font_size(30);

    attach(*score_text);
    attach(*record_name_text);
    attach(*record_score_text);
    attach(*record_time_text);
    attach(*timer_text);
    attach(*hints_text);

    create_menu_elements();
    create_record_input_elements();

    update_score_display();
    update_hints_display();
    spawn_new_figures();

    start_timer();
}

GameWindow::~GameWindow() {
    stop_timer();

    if (menu_background) {
        detach(*menu_background);
        delete menu_background;
    }
    if (game_over_text) {
        detach(*game_over_text);
        delete game_over_text;
    }
    if (restart_button) {
        detach(*restart_button);
        delete restart_button;
    }
    if (quit_button) {
        detach(*quit_button);
        delete quit_button;
    }
    if (restart_text) {
        detach(*restart_text);
        delete restart_text;
    }
    if (quit_text) {
        detach(*quit_text);
        delete quit_text;
    }
    if (hint_button) {
        detach(*hint_button);
        delete hint_button;
    }
    if (hint_button_text) {
        detach(*hint_button_text);
        delete hint_button_text;
    }
    if (timer_text) {
        detach(*timer_text);
        delete timer_text;
    }
    if (hints_text) {
        detach(*hints_text);
        delete hints_text;
    }
    if (record_name_text) {
        detach(*record_name_text);
        delete record_name_text;
    }
    if (record_score_text) {
        detach(*record_score_text);
        delete record_score_text;
    }
    if (record_time_text) {
        detach(*record_time_text);
        delete record_time_text;
    }
}

void GameWindow::draw_background() {
    const int width = x_max();
    const int height = y_max();
    const int stripe_width = 6;

    for (int x = 0; x < width; x += stripe_width) {
        const double ratio = static_cast<double>(x) / width;
        const int red = 10 + static_cast<int>(70 * ratio);
        const int green = 40 + static_cast<int>(100 * ratio);
        const int blue = 70 + static_cast<int>(130 * ratio);
        fl_color(red, green, blue);
        fl_rectf(x, 0, stripe_width, height);
    }
}

void GameWindow::create_record_input_elements() {
    int menu_width = 350;
    int menu_height = 250;
    int menu_x = (x_max() - menu_width) / 2;
    int menu_y = (y_max() - menu_height) / 2;

    name_input_bg = new Graph_lib::Rectangle(Graph_lib::Point(menu_x, menu_y), menu_width, menu_height);
    name_input_bg->set_color(Graph_lib::Color::black);
    name_input_bg->set_fill_color(Graph_lib::Color(50, 90, 140));

    name_prompt_text = new Graph_lib::Text(Graph_lib::Point(menu_x + 100, menu_y + 60), "Новый рекорд!");
    name_prompt_text->set_color(Graph_lib::Color::white);
    name_prompt_text->set_font_size(18);

    name_text = new Graph_lib::Text(Graph_lib::Point(menu_x + 100, menu_y + 110), "Введите имя...");
    name_text->set_color(Graph_lib::Color::black);
    name_text->set_font_size(16);

    submit_button = new Graph_lib::Rectangle(Graph_lib::Point(menu_x + 125, menu_y + 160), 100, 40);
    submit_button->set_color(Graph_lib::Color::black);
    submit_button->set_fill_color(Graph_lib::Color(100, 200, 100));

    submit_text = new Graph_lib::Text(Graph_lib::Point(menu_x + 155, menu_y + 185), "Готово");
    submit_text->set_color(Graph_lib::Color::black);
    submit_text->set_font_size(16);
}

int GameWindow::handle(int event) {
    int mx = Fl::event_x();
    int my = Fl::event_y();

    if (showing_menu && record_beaten && editing_name) {
        switch (event) {
        case FL_PUSH:
            if (submit_button && is_point_in_rectangle(mx, my,
                submit_button->point(0).x, submit_button->point(0).y,
                submit_button->width(), submit_button->height())) {
                submit_name();
                return 1;
            }
            break;

        case FL_KEYDOWN:
            int key = Fl::event_key();

            if (key == FL_Enter || key == FL_KP_Enter) {
                submit_name();
                return 1;
            }
            else if (key == FL_BackSpace) {
                if (!player_name.empty()) {
                    player_name.pop_back();
                }
            }
            else if ((key >= 'a' && key <= 'z') ||
                     (key >= 'A' && key <= 'Z') ||
                     (key >= '0' && key <= '9') ||
                     key == ' ' || key == '_' || key == '-') {
                if (player_name.length() < 15) {
                    player_name += static_cast<char>(key);
                }
            }

            if (player_name.empty()) {
                name_text->set_label("Введите имя...");
            } else {
                name_text->set_label(player_name);
            }

            redraw();
            return 1;
        }
        return 1;
    }

    if (showing_menu) {
        switch (event) {
        case FL_PUSH:
            if (restart_button && is_point_in_rectangle(mx, my,
                restart_button->point(0).x, restart_button->point(0).y,
                restart_button->width(), restart_button->height())) {
                cb_restart(nullptr, this);
                return 1;
            }
            if (quit_button && is_point_in_rectangle(mx, my,
                quit_button->point(0).x, quit_button->point(0).y,
                quit_button->width(), quit_button->height())) {
                cb_quit(nullptr, this);
                return 1;
            }
            break;
        }
        return 1;
    }

    switch (event) {
    case FL_PUSH:
        if (hints_remaining > 0 && hint_button &&
            is_point_in_rectangle(mx, my, hint_button->point(0).x, hint_button->point(0).y,
                                hint_button->width(), hint_button->height())) {
            cb_hint(nullptr, this);
            return 1;
        }

        for (auto& fig : figures) {
            if (fig && fig->is_available() && fig->contains(mx, my)) {
                active = fig.get();
                active->start_drag(mx, my);
                hint_used_this_turn = false;
                return 1;
            }
        }
        break;

    case FL_DRAG:
        if (active) {
            active->drag_to(mx, my);
            return 1;
        }
        break;

    case FL_RELEASE:
        if (active) {
            active->stop_drag();
            if (board.can_place(*active)) {
                int points = board.place(*active);
                current_score += points;
                generator.update_score();
                update_score_display();

                active->consume();
                active = nullptr;
                cleanup_consumed_figures();
                if (figures.empty()) {
                    spawn_new_figures();
                }

                check_game_over();
            } else {
                active->move_back_to_start();
                active = nullptr;
                check_game_over();
            }
            return 1;
        }
        break;
    }

    return Graph_lib::Window::handle(event);
}

void GameWindow::create_menu_elements() {
    int menu_width = 300;
    int menu_height = 200;
    int menu_x = (x_max() - menu_width) / 2;
    int menu_y = (y_max() - menu_height) / 2;

    menu_background = new Graph_lib::Rectangle(Graph_lib::Point(menu_x, menu_y), menu_width, menu_height);
    menu_background->set_color(Graph_lib::Color::black);
    menu_background->set_fill_color(Graph_lib::Color(40, 70, 122));

    game_over_text = new Graph_lib::Text(Graph_lib::Point(menu_x + 60, menu_y + 50), "Упс, вы проиграли!");
    game_over_text->set_color(Graph_lib::Color::white);
    game_over_text->set_font_size(24);

    restart_button = new Graph_lib::Rectangle(Graph_lib::Point(menu_x + 50, menu_y + 100), 100, 40);
    restart_button->set_color(Graph_lib::Color::black);
    restart_button->set_fill_color(Graph_lib::Color(100, 200, 100));

    restart_text = new Graph_lib::Text(Graph_lib::Point(menu_x + 75, menu_y + 125), "Restart");
    restart_text->set_color(Graph_lib::Color::black);
    restart_text->set_font_size(16);

    quit_button = new Graph_lib::Rectangle(Graph_lib::Point(menu_x + 150, menu_y + 100), 100, 40);
    quit_button->set_color(Graph_lib::Color::black);
    quit_button->set_fill_color(Graph_lib::Color(200, 100, 100));

    quit_text = new Graph_lib::Text(Graph_lib::Point(menu_x + 185, menu_y + 125), "Quit");
    quit_text->set_color(Graph_lib::Color::white);
    quit_text->set_font_size(16);

    int hint_button_x = 50;
    int hint_button_y = 550;

    hint_button = new Graph_lib::Rectangle(Graph_lib::Point(hint_button_x, hint_button_y), 150, 50);
    hint_button->set_color(Graph_lib::Color::black);
    hint_button->set_fill_color(Graph_lib::Color(100, 100, 200));
    attach(*hint_button);

    hint_button_text = new Graph_lib::Text(Graph_lib::Point(hint_button_x + 25, hint_button_y + 30), "Подсказка (3)");
    hint_button_text->set_color(Graph_lib::Color::white);
    hint_button_text->set_font_size(16);
    attach(*hint_button_text);
}

bool GameWindow::is_point_in_rectangle(int x, int y, int rect_x, int rect_y, int rect_w, int rect_h) {
    return x >= rect_x && x <= rect_x + rect_w &&
           y >= rect_y && y <= rect_y + rect_h;
}

void GameWindow::check_game_over() {
    if (showing_menu) return;

    bool has_available_move = false;

    for (const auto& fig : figures) {
        if (fig && fig->is_available()) {
            if (board.has_available_move(*fig)) {
                has_available_move = true;
                break;
            }
        }
    }

    if (!has_available_move && !figures.empty()) {
        show_game_over_menu();
    }
}

void GameWindow::show_game_over_menu() {
    if (showing_menu) return;

    game_over = true;
    showing_menu = true;
    stop_timer();

    record_beaten = (current_score > record_score);

    if (record_beaten) {
        update_record(current_score);
        record_score = get_record();
        std::ostringstream oss;
        oss << "НОВЫЙ РЕКОРД!\n" << current_score << " очков!";
        game_over_text->set_label(oss.str());
        show_record_input();
        editing_name = true;
        player_name = "";
        name_text->set_label("Введите имя...");

    } else {
        game_over_text->set_label("Упс, вы проиграли!");
        attach(*menu_background);
        attach(*game_over_text);
        attach(*restart_button);
        attach(*restart_text);
        attach(*quit_button);
        attach(*quit_text);

        hide_record_input();
        editing_name = false;
    }
    redraw();
}

void GameWindow::show_record_input() {
    attach(*name_input_bg);
    attach(*name_prompt_text);
    attach(*name_text);
    attach(*submit_button);
    attach(*submit_text);
    redraw();
}

void GameWindow::hide_record_input() {
    if (name_input_bg) detach(*name_input_bg);
    if (name_prompt_text) detach(*name_prompt_text);
    if (name_text) detach(*name_text);
    if (submit_button) detach(*submit_button);
    if (submit_text) detach(*submit_text);
}

void GameWindow::load_best_record() {
    std::ifstream hs_file("highscores.txt");
    if (!hs_file) {
        return;
    }

    int highest_score = 0;
    std::string best_name = "No record";
    std::string best_time = "00:00";

    std::string line;
    while (std::getline(hs_file, line)) {
        std::istringstream iss(line);
        std::string name, score_str, time_str;

        if (std::getline(iss, name, ';') &&
            std::getline(iss, score_str, ';') &&
            std::getline(iss, time_str)) {

            try {
                int score = std::stoi(score_str);
                if (score > highest_score) {
                    highest_score = score;
                    best_name = name;
                    best_time = time_str;
                }
            } catch (...) {
            }
        }
    }

    record_score = highest_score;
    if (record_name_text) {
        record_name_text->set_label("Record: " + best_name);
    }
    if (record_score_text) {
        record_score_text->set_label("Score: " + std::to_string(highest_score));
    }
    if (record_time_text) {
        record_time_text->set_label("Time: " + best_time);
    }
}

void GameWindow::save_highscore(const std::string& name) {
    const std::string filename = "highscores.txt";
    std::ofstream outFile(filename, std::ios::app);

    if (outFile) {
        int minutes = elapsed_seconds / 60;
        int seconds = elapsed_seconds % 60;
        std::string time_str = std::to_string(minutes) + ":" +
                              (seconds < 10 ? "0" : "") + std::to_string(seconds);

        outFile << name << ";"
                << current_score << ";"
                << time_str
                << std::endl;

        load_best_record();

    } else {
        std::cerr << "Не удалось сохранить рекорд" << std::endl;
    }
}

void GameWindow::submit_name() {
    std::string name = player_name;

    if (name.empty()) {
        name = "anonim";
    }
    save_highscore(name);
    editing_name = false;

    hide_record_input();

    game_over_text->set_label("Упс, вы проиграли!");

    attach(*menu_background);
    attach(*game_over_text);
    attach(*restart_button);
    attach(*restart_text);
    attach(*quit_button);
    attach(*quit_text);

    redraw();
}
void GameWindow::hide_game_over_menu() {
    showing_menu = false;

    if (menu_background) detach(*menu_background);
    if (game_over_text) detach(*game_over_text);
    if (restart_button) detach(*restart_button);
    if (restart_text) detach(*restart_text);
    if (quit_button) detach(*quit_button);
    if (quit_text) detach(*quit_text);
    hide_record_input();

    board.update_display();
    redraw();
}

int GameWindow::get_record(){
    const std::string filename = "data.txt";
    std::ifstream inFile(filename);
    int value = 0;
    if (inFile) {
        if (inFile >> value) {
            return value;
        }
    }
    return 0;
}

void GameWindow::update_record(int new_value) {
    const std::string filename = "data.txt";
    std::ifstream inFile(filename);
    int old_value;

    bool readSuccess = false;
    if (inFile) {
        if (inFile >> old_value) {
            readSuccess = true;
        }
    }

    if (!readSuccess || new_value > old_value) {
        std::ofstream outFile(filename);
        if (outFile) {
            outFile << new_value;
        }
    }
}

void GameWindow::cb_timer(void* data) {
    GameWindow* win = static_cast<GameWindow*>(data);
    win->timer_tick();
}

void GameWindow::start_timer() {
    elapsed_seconds = 0;
    update_timer_display();
    Fl::add_timeout(1.0, cb_timer, this);
}

void GameWindow::stop_timer() {
    Fl::remove_timeout(cb_timer, this);
}

void GameWindow::timer_tick() {
    if (showing_menu) return;

    elapsed_seconds++;
    update_timer_display();
    Fl::repeat_timeout(1.0, cb_timer, this);
}

void GameWindow::update_timer_display() {
    int minutes = elapsed_seconds / 60;
    int seconds = elapsed_seconds % 60;

    std::ostringstream oss;
    oss << "Time: " << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds;

    timer_text->set_label(oss.str());

    if (elapsed_seconds >= 180) {
        timer_text->set_color(FL_RED);
    } else if (elapsed_seconds >= 120) {
        timer_text->set_color(Graph_lib::Color(255, 255, 0));
    } else {
        timer_text->set_color(Graph_lib::Color::white);
    }

    redraw();
}

void GameWindow::update_hints_display() {
    hints_text->set_label("Hints: " + std::to_string(hints_remaining));

    if (hint_button_text) {
        hint_button_text->set_label("Подсказка (" + std::to_string(hints_remaining) + ")");
    }

    if (hint_button) {
        if (hints_remaining > 0) {
            hint_button->set_fill_color(Graph_lib::Color(100, 100, 200));
        } else {
            hint_button->set_fill_color(Graph_lib::Color(100, 100, 100));
        }
    }

    redraw();
}

void GameWindow::use_hint() {
    if (hints_remaining <= 0 || showing_menu) {
        return;
    }

    int old_score = current_score;

    if (find_and_place_best_move()) {
        hints_remaining--;
        update_hints_display();

        if (current_score > static_cast<unsigned int>(old_score)) {
            hint_used_this_turn = false;
        }

    }
}

bool GameWindow::find_and_place_best_move() {
    struct MoveInfo {
        DraggableFigure* fig;
        int grid_x;
        int grid_y;
        int score;
        int rows_cleared;
        int cols_cleared;
    };

    std::vector<MoveInfo> possible_moves;

    for (auto& fig : figures) {
        if (fig && fig->is_available()) {
            auto rel_positions = fig->get_relative_positions();

            for (int grid_y = 0; grid_y < GRID_SIZE; grid_y++) {
                for (int grid_x = 0; grid_x < GRID_SIZE; grid_x++) {
                    bool can_place_here = true;

                    for (const auto& rel_pos : rel_positions) {
                        int check_x = grid_x + rel_pos.first;
                        int check_y = grid_y + rel_pos.second;

                        if (check_x < 0 || check_x >= GRID_SIZE ||
                            check_y < 0 || check_y >= GRID_SIZE) {
                            can_place_here = false;
                            break;
                        }

                        ::Coordinates coord(check_x, check_y);
                        if (!generator.get_field().is_free(coord)) {
                            can_place_here = false;
                            break;
                        }
                    }

                    if (can_place_here) {
                        MoveInfo move;
                        move.fig = fig.get();
                        move.grid_x = grid_x;
                        move.grid_y = grid_y;

                        move.score = rel_positions.size() * 10;

                        move.rows_cleared = 0;
                        move.cols_cleared = 0;

                        std::vector<std::vector<bool>> simulated_field(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

                        for (int r = 0; r < GRID_SIZE; r++) {
                            for (int c = 0; c < GRID_SIZE; c++) {
                                ::Coordinates coord(c, r);
                                simulated_field[r][c] = !generator.get_field().is_free(coord);
                            }
                        }

                        for (const auto& rel_pos : rel_positions) {
                            int cell_x = grid_x + rel_pos.first;
                            int cell_y = grid_y + rel_pos.second;
                            simulated_field[cell_y][cell_x] = true;
                        }

                        for (int r = 0; r < GRID_SIZE; r++) {
                            bool row_full = true;
                            for (int c = 0; c < GRID_SIZE; c++) {
                                if (!simulated_field[r][c]) {
                                    row_full = false;
                                    break;
                                }
                            }
                            if (row_full) {
                                move.rows_cleared++;
                                move.score += 100;
                            }
                        }

                        for (int c = 0; c < GRID_SIZE; c++) {
                            bool col_full = true;
                            for (int r = 0; r < GRID_SIZE; r++) {
                                if (!simulated_field[r][c]) {
                                    col_full = false;
                                    break;
                                }
                            }
                            if (col_full) {
                                move.cols_cleared++;
                                move.score += 100;
                            }
                        }

                        int adjacent_blocks = 0;
                        for (const auto& rel_pos : rel_positions) {
                            int cell_x = grid_x + rel_pos.first;
                            int cell_y = grid_y + rel_pos.second;

                            int dx[] = {-1, 1, 0, 0};
                            int dy[] = {0, 0, -1, 1};

                            for (int i = 0; i < 4; i++) {
                                int nx = cell_x + dx[i];
                                int ny = cell_y + dy[i];

                                if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE) {
                                    if (simulated_field[ny][nx]) {
                                        adjacent_blocks++;
                                    }
                                }
                            }
                        }

                        move.score += adjacent_blocks * 5;

                        bool near_edge = false;
                        for (const auto& rel_pos : rel_positions) {
                            int cell_x = grid_x + rel_pos.first;
                            int cell_y = grid_y + rel_pos.second;

                            if (cell_x == 0 || cell_x == GRID_SIZE-1 ||
                                cell_y == 0 || cell_y == GRID_SIZE-1) {
                                near_edge = true;
                                break;
                            }
                        }

                        if (near_edge) {
                            move.score += 20;
                        }
                        possible_moves.push_back(move);
                    }
                }
            }
        }
    }

    if (possible_moves.empty()) {
        return false;
    }

    std::sort(possible_moves.begin(), possible_moves.end(),
              [](const MoveInfo& a, const MoveInfo& b) {
                  return a.score > b.score;
              });

    std::vector<MoveInfo> top_moves;
    for (size_t i = 0; i < std::min(possible_moves.size(), size_t(3)); i++) {
        top_moves.push_back(possible_moves[i]);
    }

    if (!top_moves.empty()) {
        int random_index = std::rand() % top_moves.size();
        MoveInfo& best_move = top_moves[random_index];

        int start_x = best_move.fig->start_base_x;
        int start_y = best_move.fig->start_base_y;
        int target_x = GridBoard::OFFSET_X + best_move.grid_x * CELL;
        int target_y = GridBoard::OFFSET_Y + best_move.grid_y * CELL;

        best_move.fig->start_drag(start_x, start_y);
        best_move.fig->drag_to(target_x, target_y);
        best_move.fig->stop_drag();

        if (board.can_place(*best_move.fig)) {
            int points = board.place(*best_move.fig);
            current_score += points;
            generator.update_score();
            update_score_display();

            best_move.fig->consume();
            cleanup_consumed_figures();

            if (figures.empty()) {
                spawn_new_figures();
            }

            return true;
        }
    }

    return false;
}

void GameWindow::restart_game() {
    game_over = false;
    stop_timer();

    figures.clear();
    active = nullptr;

    update_record(current_score);
    load_best_record();

    current_score = 0;
    update_score_display();

    elapsed_seconds = 0;

    hints_remaining = 3;
    hint_used_this_turn = false;
    update_hints_display();

    generator.~Game();
    new (&generator) Game(GRID_SIZE, GRID_SIZE);
    generator.initialize_types_of_blocks();

    board.game_field = &generator.get_field();
    board.update_display();

    hide_game_over_menu();

    start_timer();
    spawn_new_figures();
}

void GameWindow::quit_game() {
    hide();
}

void GameWindow::spawn_new_figures() {
    auto blocks = generator.generate_blocks();
    if (blocks.empty()) {
        show_game_over_menu();
        return;
    }

    figures.clear();
    figures.reserve(FIGURE_SLOTS);

    for (int i = 0; i < FIGURE_SLOTS && i < static_cast<int>(blocks.size()); ++i) {
        Graph_lib::Color color{color_palette[i % color_palette.size()]};
        figures.push_back(build_figure(blocks[i], spawn_positions[i], color));
    }

    check_game_over();
    hint_used_this_turn = false;
}

std::unique_ptr<DraggableFigure> GameWindow::build_figure(const ::Block& block, Graph_lib::Point anchor, Graph_lib::Color color) {
    auto fig = std::make_unique<DraggableFigure>(this);
    const auto& positions = block.get_positions();
    if (positions.empty()) return fig;

    int min_x = positions[0].x;
    int min_y = positions[0].y;
    for (const auto& pos : positions) {
        min_x = std::min(min_x, pos.x);
        min_y = std::min(min_y, pos.y);
    }

    for (const auto& pos : positions) {
        int px = anchor.x + (pos.x - min_x) * CELL;
        int py = anchor.y + (pos.y - min_y) * CELL;
        fig->add_block(Graph_lib::Point{px, py}, CELL, color);
    }

    fig->save_start();
    return fig;
}

void GameWindow::cleanup_consumed_figures() {
    figures.erase(
        std::remove_if(figures.begin(), figures.end(),
                       [](const std::unique_ptr<DraggableFigure>& fig) {
                           return !fig->is_available();
                       }),
        figures.end());
}

void GameWindow::update_score_display() {
    score_text->set_label("Score: " + std::to_string(current_score));
    redraw();
}

void GameWindow::cb_restart(Graph_lib::Address, Graph_lib::Address addr) {
    auto* win = static_cast<GameWindow*>(addr);
    win->restart_game();
}

void GameWindow::cb_quit(Graph_lib::Address, Graph_lib::Address addr) {
    auto* win = static_cast<GameWindow*>(addr);
    win->quit_game();
}

void GameWindow::cb_hint(Graph_lib::Address, Graph_lib::Address addr) {
    auto* win = static_cast<GameWindow*>(addr);
    win->use_hint();
}

} // namespace BlockBlast