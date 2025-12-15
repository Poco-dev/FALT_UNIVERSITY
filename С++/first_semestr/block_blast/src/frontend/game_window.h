#ifndef GAME_WINDOW_GUARD
#define GAME_WINDOW_GUARD

#include <vector>
#include <array>
#include <memory>
#include <string>
#include "Graph_lib/Window.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/GUI.h"
#include "../backend/game.h"
#include "grid_board.h"
#include "constants.h"

namespace BlockBlast {

struct GameWindow : Graph_lib::Window {
    GridBoard board;
    ::Game generator;
    std::vector<std::unique_ptr<DraggableFigure>> figures;
    DraggableFigure* active = nullptr;
    const std::array<Graph_lib::Point, FIGURE_SLOTS> spawn_positions;
    const std::array<Graph_lib::Color, 6> color_palette;

    unsigned int current_score;
    Graph_lib::Text* score_text;
    Graph_lib::Text* record_name_text = nullptr;
    Graph_lib::Text* record_score_text = nullptr;
    Graph_lib::Text* record_time_text = nullptr;
    Graph_lib::Text* timer_text;
    Graph_lib::Text* hints_text;
    unsigned int record_score;
    int elapsed_seconds;
    int hints_remaining;
    bool hint_used_this_turn;
    bool record_beaten = false;
    std::string player_name = "";
    bool editing_name = false;
    bool game_over = false;
    bool showing_menu = false;

    Graph_lib::Rectangle* menu_background = nullptr;
    Graph_lib::Text* game_over_text = nullptr;
    Graph_lib::Rectangle* restart_button = nullptr;
    Graph_lib::Rectangle* quit_button = nullptr;
    Graph_lib::Text* restart_text = nullptr;
    Graph_lib::Text* quit_text = nullptr;


    Graph_lib::Rectangle* hint_button = nullptr;
    Graph_lib::Text* hint_button_text = nullptr;

    Graph_lib::Rectangle* name_input_bg = nullptr;
    Graph_lib::Text* name_prompt_text = nullptr;
    Graph_lib::Text* name_text = nullptr;
    Graph_lib::Rectangle* submit_button = nullptr;
    Graph_lib::Text* submit_text = nullptr;

    static void cb_restart(Graph_lib::Address, Graph_lib::Address);
    static void cb_quit(Graph_lib::Address, Graph_lib::Address);
    static void cb_hint(Graph_lib::Address, Graph_lib::Address);
    static void cb_timer(void*);

    GameWindow(Graph_lib::Point p, int w, int h, const std::string& title = "BlockBlast");
    ~GameWindow();

protected:
    void draw_background() override;
    int handle(int event) override;

private:
    void create_menu_elements();
    void create_record_input_elements();
    bool is_point_in_rectangle(int x, int y, int rect_x, int rect_y, int rect_w, int rect_h);
    void check_game_over();
    void show_game_over_menu();
    void hide_game_over_menu();
    void show_record_input();
    void hide_record_input();
    void load_best_record();
    int get_record();
    void update_record(int new_value);
    void restart_game();
    void quit_game();
    void spawn_new_figures();
    std::unique_ptr<DraggableFigure> build_figure(const ::Block& block, Graph_lib::Point anchor, Graph_lib::Color color);
    void cleanup_consumed_figures();
    void update_score_display();
    void update_timer_display();
    void update_hints_display();
    void start_timer();
    void stop_timer();
    void timer_tick();
    void use_hint();
    bool find_and_place_best_move();
    void save_highscore(const std::string& name);
    void submit_name();
    void process_key_for_name(int key);
};

} // namespace BlockBlast
#endif