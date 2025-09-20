#include <iostream>
#include <random>
#include <vector>
using namespace std;

void init_func();
int randomaizer();
vector<int> generate_heaps();
void single_game();
void nim_game();
int computer_move_single(int stones);
int player_move_single(int stones);
vector<int> computer_move_nim(vector<int> heaps);
vector<int> player_move_nim(vector<int> heaps);
void print_heaps(const vector<int>& heaps);

constexpr int heaps_count{3};
const string stone_img{"🪨"};

int main() {
    init_func();
    cout << "Выберите режим игры:" << endl;
    cout << "1 - Одна куча (простой Ним)" << endl;
    cout << "2 - Три кучи (классический Ним)" << endl;

    char choice{'0'};
    cin >> choice;

    if (choice == '1') {
        single_game();
    } else if (choice == '2') {
        nim_game();
    }
    return 0;
}
//рандомайзер от 5 до 10
int randomaizer() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(5, 10);
    return dis(gen);
}
//добавление возможности выводить консоль Кириллицу(русский язык)
void init_func() {
    system("chcp 65001>nul");
}
//игра на 1 кучу
void single_game() {
    int stones_count = randomaizer();
    bool player_move = false;

    cout << "Кто ходит первым?" << "\n" << "Вы?(y/n)" << endl;
    string word{"123"};
    cin >> word;

    if (tolower(word[0]) == 'y')
        player_move = true;

    cout << "Количество камней в куче: " << endl;
    for (int i{0}; i < stones_count; ++i) {
        cout << stone_img;
    }
    cout << endl;

    while (stones_count > 0) {
        if (player_move) {
            stones_count = player_move_single(stones_count);
            for (int i{0}; i < stones_count; ++i) {
                cout << stone_img;
            }
            cout << endl;
        } else {
            stones_count = computer_move_single(stones_count);
            cout << "В куче осталось " << stones_count << " камней" << endl;
            for (int i{0}; i < stones_count; ++i) {
                cout << stone_img;
            }
            cout << endl;
        }

        if (stones_count == 0) {
            if (player_move) {
                cout << "Вы победили! Ура!" << endl;
            } else {
                cout << "Вы проиграли. Повезёт в следующий раз!" << endl;
            }
            break;
        }

        player_move = !player_move;
    }
}
//ход компьютера в игре на 1 кучу
int computer_move_single(int stones) {
    int taked_stones = stones % 4;
    if (taked_stones == 0) {
        taked_stones = 1;
    }

    cout << "Компьютер взял " << taked_stones << " камней" << endl;
    return stones - taked_stones;
}
//ход игрока на 1 кучу
int player_move_single(int stones) {
    int choose_count{0};
    cout << "Ваш ход. Сколько камней возьмете? (1-" << min(3,stones) << ")"<< endl;

    while (cin >> choose_count &&
        (choose_count < 1 || choose_count > 3 || choose_count > stones))
    {
        cerr << "Некорректное количество. Введите еще раз (1-" << min(3,stones) << "):" << endl;
    }

    return stones - choose_count;
}
//игра на 3 кучи
void nim_game() {
    vector<int> heaps = generate_heaps();
    bool player_move = false;

    cout << "Кто ходит первым?" << "\n" << "Вы?(y/n)" << endl;
    string word;
    cin >> word;

    if (word == "y" || word == "Yes" || word == "yes" || word == "Y")
        player_move = true;

    cout << "Количество камней в кучах: " << endl;
    print_heaps(heaps);

    while (true) {
        if (player_move) {
            heaps = player_move_nim(heaps);
            cout << "Осталось камней: " << endl;
            print_heaps(heaps);
        } else {
            heaps = computer_move_nim(heaps);
            cout << "Осталось камней: " << endl;
            print_heaps(heaps);
        }

        if (heaps[0] == 0 && heaps[1] == 0 && heaps[2] == 0) {
            if (player_move) {
                cout << "Вы победили! Ура!" << endl;
            } else {
                cout << "Вы проиграли. Повезёт в следующий раз!" << endl;
            }
            break;
        }

        player_move = !player_move;
    }
}
//генератор 3куч
vector<int> generate_heaps() {
    vector<int> heaps(heaps_count);
    for (int i{0}; i < heaps_count; ++i) {
        heaps[i] = randomaizer();
    }
    return heaps;
}
//ход компьютера в игре на 3 кучи(XOR-Алгоритм)
vector<int> computer_move_nim(vector<int> heaps) {
    int nim_sum = heaps[0] ^ heaps[1] ^ heaps[2];
    if (nim_sum == 0) {
        for (int i{0}; i < heaps_count; ++i) {
            if (heaps[i] > 0) {
                heaps[i] = max(0, heaps[i] - 1);
                cout << "Компьютер взял 1 камень из кучи " << i + 1 << endl;
                return heaps;
            }
        }
    }
    for (int i{0}; i < heaps_count; ++i) {
        if (const int target = heaps[i] ^ nim_sum; target < heaps[i]) {
            const int take = heaps[i] - target;
            heaps[i] = target;
            cout << "Компьютер взял " << take << " камней из кучи " << i + 1 << endl;
            return heaps;
        }
    }

    return heaps;
}
//Ход игрока в игре на 3 кучи
vector<int> player_move_nim(vector<int> heaps) {
    unsigned int heap_num{}, take_count{};

    cout << "Ваш ход. Выберите кучу (1-3):" << endl;
    cin >> heap_num;

    while (heap_num < 1 || heap_num > 3 || heaps[heap_num - 1] == 0) {
        cerr << "Некорректная куча. Выберите еще раз (1-3):" << endl;
        cin >> heap_num;
    }

    cout << "Сколько камней возьмете из кучи " << heap_num << "? (1-" << heaps[heap_num - 1] << ")" << endl;
    cin >> take_count;

    while (take_count < 1 || take_count > heaps[heap_num - 1]) {
        cerr << "Некорректное количество. Введите еще раз:";
        cin >> take_count;
    }

    heaps[heap_num - 1] -= take_count;
    return heaps;
}
//Функция для вывода камней(исключительно в debug)
void print_heaps(const vector<int>& heaps) {
    for (int i{0}; i < heaps_count; ++i) {
        cout << "(" << heaps[i] << ")";
        for (int j{0}; j<heaps[i]; ++j) {
            cout << " 🪨 ";
        }
        cout << endl;
    }
}