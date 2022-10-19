#ifndef SYSTEM_H
#define SYSTEM_H

using namespace std;

#include <ctime>
#include <curses.h>
#include <ncurses.h>
#include <omp.h>
#include <bits/stdc++.h>

#include "Player.h"
#include "Map.h"
#include "Scoreboard.h"

class System
{
private:
    bool is_playing, is_paused;
    long double current_time;
    Scoreboard scoreboard;
    Player player_1, player_2;
    Map map;

public:
    System()
    {
        is_playing = true;
        is_paused = false;
        current_time = 0.0;
        Player player_1, player_2;
        Map map;
        Scoreboard scoreboard;
    }

    // Methods
    void __init__() // Inicializa todos as consfigurações necessárias
    {
        player_1.set_name("Player 1");
        player_1.set_keyboard(119, 115, 97, 100); // WSAD -> Nesta ordem
        player_1.set_view(3);

        player_2.set_name("Player 2");
        player_2.set_keyboard(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
        player_2.set_view(3);

        scoreboard.set_size(5);
        scoreboard.set_scores();

        main_menu();
    }

    void main_menu()
    {
        clear();
        printw("+-----------------------------------------------------+\n");
        printw("|                    Welcome to the                   |\n");
        printw("|                                                     |\n");
        printw("|              Fehl Sky Maze - Lost in the            |\n");
        printw("|                  Fractured But Whole                |\n");
        printw("|                                                     |\n");
        printw("+-----------------------------------------------------+\n");
        printw("\n");
        printw("+-----------------------------------------------------+\n");
        printw("| - Press 1 to Start a New Game                       |\n");
        printw("| - Press 2 to Continue Saved Game                    |\n");
        printw("| - Press 3 to open the Scoreboard                    |\n");
        printw("| - Press 4 to Quit                                   |\n");
        printw("+-----------------------------------------------------+\n");
        printw("\n");
        refresh();

        int input;
        do
        {
            input = getch() - 48;
            switch (input)
            {
            case 1:
                set_game("maze.txt", 20, 24);
                break;
            case 2:
                set_game("saved_game.txt", 20, 24);
                break;
            case 3:
                scoreboard.show_scoreboard();
                int input;
                do
                {
                    input = getch() - 48;
                    switch (input)
                    {
                    case 1:
                        main_menu();
                        break;
                    default:
                        printw("Please, inform a valid selection\n");
                        break;
                    }
                } while (input <= 0 || input >= 2);
                break;
            case 4:
                is_playing = false;
                printw("Exiting game....");
                break;
            default:
                printw("Please, inform a valid selection\n");
                break;
            }
        } while (input <= 0 || input >= 5);
    }

    void set_game(string name, int size_x, int size_y)
    {
        map.set_name(name);
        map.set_size(size_x, size_y);
        map.load_file();
        map.define_players_initial_coords(2);

        player_1.set_coord(map.get_players_coords()[0]);
        player_2.set_coord(map.get_players_coords()[1]);

        is_playing = true;

        game(map, player_1, player_2);
    }

    void game(Map map, Player &player_1, Player &player_2)
    {
        clock_t start;
        start = clock();

        nodelay(stdscr, TRUE); // Enable the getch to be non-blocking
        while (is_playing)
        {
            erase();
            time(start);
            show_map(map, player_1, 1);
            show_map_transposed(map, player_2, 20);

            win_game(current_time, 3, player_1, player_2);

            int input = getch();
            player_1.move(input, is_playing, is_paused, map);
            if (!is_playing) // Cláusula de Guarda
                break;

            player_2.move(input, is_playing, is_paused, map);

            // Valida e executa power ups para um player
            if (player_2.get_last_position_value() == 3)
            {
                player_2.set_last_position_value(0); // Usado para evitar loop infinito
                randomize_power(map, player_1, player_2);
            }
        }

        if (is_paused)
        {
            pause_game();
        }
        nodelay(stdscr, FALSE); // Disable the getch to be non-blocking
    }

    void time(clock_t start)
    {
        clock_t sum = 33.33;
        current_time = (clock() - start + sum) / (double)CLOCKS_PER_SEC;
        move(10, 10);
        printw("Clock: %Lf", current_time);
    }

    void pause_game()
    {
        nodelay(stdscr, FALSE); // Disable the getch to be non-blocking

        clear();
        printw("+----------------------------------------------+\n");
        printw("|                                              |\n");
        printw("|                 Game Paused!                 |\n");
        printw("|                                              |\n");
        printw("+----------------------------------------------+\n");
        printw("\n");
        printw("+----------------------------------------------+\n");
        printw("| - Press 1 to Start a New Game                |\n");
        printw("| - Press 2 to Continue Saved Game             |\n");
        printw("| - Press 3 to Save                            |\n");
        printw("| - Press 4 to Quit                            |\n");
        printw("+----------------------------------------------+\n");
        printw("\n");
        refresh();

        int input;
        do
        {
            input = getch() - 48;
            switch (input)
            {
            case 1:
                set_game("maze.txt", 20, 24);
                break;
            case 2:
                set_game("saved_game.txt", 20, 24);
                break;
            case 3:
                map.save_file();
                printw("Game Saved!\n");
                pause_game();
                break;
            case 4:
                is_playing = false;
                printw("Exiting game....");
                break;
            default:
                printw("Please, inform a valid selection\n");
                break;
            }
        } while (input <= 0 || input >= 6);
    }

    void win_game(long double time, int view, Player &player_1, Player &player_2)
    {
        // O método abs() retornar o valor positivo do número informado
        if (abs(player_1.get_x() - player_2.get_x()) < view && abs(player_1.get_y() - player_2.get_y()) < view) // Condição de Vitória
        {
            nodelay(stdscr, FALSE); // Disable the getch to be non-blocking
            is_playing = false;
            scoreboard.save_scoreboard(time);

            clear();
            printw("+-----------------------------------------------------+\n");
            printw("|\n");
            printw("|  The Total time was: %Lf\n", time);
            printw("|\n");
            printw("+-----------------------------------------------------+\n");
            printw("\n");
            printw("+-----------------------------------------------------+\n");
            printw("| - Press 1 to Start a New Game                       |\n");
            printw("| - Press 2 to open the Scoreboard                    |\n");
            printw("| - Press 3 to Quit                                   |\n");
            printw("+-----------------------------------------------------+\n");
            printw("\n");
            refresh();

            int input;
            do
            {
                input = getch() - 48;
                switch (input)
                {
                case 1:
                    set_game("maze.txt", 20, 24);
                    break;
                case 2:
                    scoreboard.show_scoreboard();
                    int input;
                    do
                    {
                        input = getch() - 48;
                        switch (input)
                        {
                        case 1:
                            // win_game(time, 3, player_1, player_2);
                            main_menu();
                            break;
                        default:
                            printw("Please, inform a valid selection\n");
                            break;
                        }
                    } while (input <= 0 || input >= 2);
                    break;
                case 3:
                    printw("Exiting game....\n");
                    nodelay(stdscr, TRUE); // Usado para sair do jogo sem precisar apertar o teclado 2 vezes
                    break;
                default:
                    printw("Please, inform a valid selection\n");
                    break;
                }
            } while (input <= 0 || input >= 5);
        }
    };

    void show_map(Map file, Player &player, int win_x = 0, int win_y = 2)
    {
        int file_x = file.get_x();
        int file_y = file.get_y();
        int **file_matrix = file.get_matrix();
        int player_x = player.get_x();
        int player_y = player.get_y();
        int player_view = player.get_view();

        mvprintw(0, win_x, player.get_name().c_str()); // Usa c_str para converter string para char* e mvprint para mostrar no terminal uma sequência de char
        for (int i = player_x - player_view; i < player_x + player_view; i++)
        {
            int temp_win_x = win_x;
            for (int j = player_y - player_view; j < player_y + player_view; j++)
            {
                if (i >= 0 && j >= 0 && i < file_x && j < file_y)
                {
                    switch (file_matrix[i][j])
                    {
                    case 0:
                        mvaddch(win_y, temp_win_x, ' '); // Mova o cursor para o x e y definido e adicione um caractere a essa posição
                        break;
                    case 1:
                        mvaddch(win_y, temp_win_x, char(219));
                        break;
                    case 2:
                        mvaddch(win_y, temp_win_x, char(1));
                        break;
                    }
                }
                temp_win_x++; // Usado para tornar a visualização do player fixa na lateral da tela
            }
            printw("\n");
            win_y++; // Usado para tornar a visualização do player fixa na lateral da tela
        }
        printw("\n");
    }

    void show_map_transposed(Map file, Player &player, int win_x = 0, int win_y = 2)
    {
        int file_x = file.get_x();
        int file_y = file.get_y();
        int **file_matrix = file.get_matrix();
        int player_x = player.get_x();
        int player_y = player.get_y();
        int player_view = player.get_view();

        mvprintw(0, win_x, player.get_name().c_str()); // Usa c_str para converter string para char* e mvprint para mostrar no terminal uma sequência de char

        for (int j = player_y - player_view; j < player_y + player_view; j++)
        {
            int temp_win_x = win_x;
            for (int i = player_x - player_view; i < player_x + player_view; i++)
            {
                if (i >= 0 && j >= 0 && i < file_x && j < file_y)
                {
                    switch (file_matrix[i][j])
                    {
                    case 0:
                        mvaddch(win_y, temp_win_x, ' '); // Mova o cursor para o x e y definido e adicione um caractere a essa posição
                        break;
                    case 1:
                        mvaddch(win_y, temp_win_x, char(219));
                        break;
                    case 2:
                        mvaddch(win_y, temp_win_x, char(1));
                        break;
                    case 3:
                        mvaddch(win_y, temp_win_x, char(63));
                        break;
                    }
                }
                temp_win_x++; // Usado para tornar a visualização do player fixa na lateral da tela
            }
            printw("\n");
            win_y++; // Usado para tornar a visualização do player fixa na lateral da tela
        }
        printw("\n");
    }

    void randomize_power(Map &map, Player &player_1, Player &player_2)
    {
        int lucky_number = rand() % 12;

        if (lucky_number >= 0 && lucky_number <= 3)
        {
            std::thread th(&System::stop_movement, this, std::ref(player_1));
            th.detach();
        }
        else if (lucky_number >= 4 && lucky_number <= 7)
        {
            std::thread th(&System::reduce_view, this, std::ref(player_1), 2);
            th.detach();
        }
        else if (lucky_number >= 8 && lucky_number <= 11)
        {
            std::thread th(&System::portal, this, std::ref(map), std::ref(player_2));
            th.detach();
        }
    }

    void portal(Map &map, Player &player)
    {
        int random_x, random_y;
        int **map_matrix = map.get_matrix();

        do
        {
            random_x = rand() % map.get_x();
            random_y = rand() % map.get_y();
        } while (map_matrix[random_x][random_y] != 0);

        map_matrix[player.get_x()][player.get_y()] = 0;
        map_matrix[random_x][random_y] = 2;
        int new_coords[2] = {random_x, random_y};
        player.set_coord(new_coords);
        std::this_thread::sleep_for(chrono::seconds(5)); // Para esse thread por n segundos
    }

    void reduce_view(Player &player, int new_view)
    {
        static int current_view = player.get_view();

        player.set_view(new_view);
        std::this_thread::sleep_for(chrono::seconds(5)); // Para esse thread por n segundos
        player.set_view(current_view);
    }

    void stop_movement(Player &player)
    {
        player.set_can_move(false);
        std::this_thread::sleep_for(chrono::seconds(5)); // Para esse thread por n segundos
        player.set_can_move(true);
    }
};

#endif // SYSTEM_H