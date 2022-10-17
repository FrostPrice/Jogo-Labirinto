#ifndef SYSTEM_H
#define SYSTEM_H

using namespace std;

#include <ctime>
#include <curses.h>
#include <ncurses.h>
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
        printw("| - Pressione 1 para Comecar um Novo Jogo             |\n");
        printw("| - Pressione 2 para continuar o jogo salvo           |\n");
        printw("| - Pressione 3 para Visualizar o Quadro de Pontuação |\n");
        printw("| - Pressione 4 para Sair                             |\n");
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
                        printw("Por favor, adicione uma seleção válida\n");
                        break;
                    }
                } while (input <= 0 || input >= 3);
                break;
            case 4:
                is_playing = false;
                printw("Saindo do jogo....");
                break;
            default:
                printw("Por favor, adicione uma seleção válida\n");
                break;
            }
        } while (input <= 0 || input >= 4);
    }

    void set_game(string name, int size_x, int size_y)
    {
        map.set_name(name);
        map.set_size(size_x, size_y);
        map.load_file();
        map.define_players_coords(2);

        player_1.set_coord(map.get_players_coords()[0]);
        player_2.set_coord(map.get_players_coords()[1]);

        is_playing = true;

        game(map, player_1, player_2);
    }

    void game(Map map, Player player_1, Player player_2)
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
        move(10, 30);
        printw("Clock: %Lf", current_time);
    }

    void pause_game()
    {
        nodelay(stdscr, FALSE); // Disable the getch to be non-blocking

        clear();
        printw("+----------------------------------------------+\n");
        printw("|                                              |\n");
        printw("|                 Jogo Pausado!                |\n");
        printw("|                                              |\n");
        printw("+----------------------------------------------+\n");
        printw("\n");
        printw("+----------------------------------------------+\n");
        printw("| - Pressione 1 para Iniciar um novo jogo      |\n");
        printw("| - Pressione 2 para Continuar o jogo salvo    |\n");
        printw("| - Pressione 3 para Salvar                    |\n");
        printw("| - Pressione 4 para Sair                      |\n");
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
                printw("Jogo Salvo!\n");
                pause_game();
                break;
            case 4:
                is_playing = false;
                printw("Saindo do jogo....");
                break;
            default:
                printw("Por favor, adicione uma seleção válida\n");
                break;
            }
        } while (input <= 0 || input >= 6);
    }

    void win_game(long double time, int view, Player player_1, Player player_2)
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
            printw("|  O tempo total foi: %Lf\n", time);
            printw("|\n");
            printw("+-----------------------------------------------------+\n");
            printw("\n");
            printw("+-----------------------------------------------------+\n");
            printw("| - Pressione 1 para iniciar um novo jogo             |\n");
            printw("| - Pressione 2 para Visualizar o Quadro de Pontuação |\n");
            printw("| - Pressione 3 para sair                             |\n");
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
                    break;
                case 3:
                    printw("Saindo do jogo....");
                    break;
                default:
                    printw("Por favor, adicione uma seleção válida\n");
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
                    }
                }
                temp_win_x++; // Usado para tornar a visualização do player fixa na lateral da tela
            }
            printw("\n");
            win_y++; // Usado para tornar a visualização do player fixa na lateral da tela
        }
        printw("\n");
    }
};

#endif // SYSTEM_H