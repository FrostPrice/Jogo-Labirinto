#ifndef SYSTEM_H
#define SYSTEM_H

using namespace std;

#include <ctime>
#include <curses.h>
#include <ncurses.h>
#include <bits/stdc++.h>

#include "Player.h"
#include "File.h"

class System
{
private:
    bool is_playing;

public:
    System() { is_playing = true; }

    // Methods
    void main_menu()
    {
        clear();
        printw("+--------------------------------------+\n");
        printw("|            Welcome to the            |\n");
        printw("|                                      |\n");
        printw("|     Fehl Sky Maze - Lost in the      |\n");
        printw("|         Fractured But Whole          |\n");
        printw("|                                      |\n");
        printw("+--------------------------------------+\n");
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
                start_new_game("maze.txt", 20, 24);
                break;
            case 2:
                load_game("saved_game.txt", 20, 24);
                break;
            case 3:
                show_scoreboard();
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

    void start_new_game(string name, int size_x, int size_y)
    {
        File map;
        map.set_name(name);
        map.set_size(size_x, size_y);
        map.load_file();
        map.define_players_coords(2);

        Player player_1;
        player_1.set_name("Player 1");
        player_1.set_keyboard(119, 115, 97, 100); // WSAD -> Nesta ordem
        player_1.set_coord(map.get_players_coords()[0]);
        player_1.set_view(3);

        Player player_2;
        player_2.set_name("Player 2");
        player_2.set_keyboard(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
        player_2.set_coord(map.get_players_coords()[1]);
        player_2.set_view(3);

        is_playing = true;

        game(map, player_1, player_2);
    }

    void load_game(string name, int size_x, int size_y)
    {
        File map;
        map.set_name(name);
        map.set_size(size_x, size_y);
        map.load_file();
        map.define_players_coords(2);

        Player player_1;
        player_1.set_name("Player 1");
        player_1.set_keyboard(119, 115, 97, 100); // WSAD -> Nesta ordem
        player_1.set_coord(map.get_players_coords()[0]);
        player_1.set_view(3);

        Player player_2;
        player_2.set_name("Player 2");
        player_2.set_keyboard(KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN);
        player_2.set_coord(map.get_players_coords()[1]);
        player_2.set_view(3);

        is_playing = true;

        game(map, player_1, player_2);
    };

    void pause_game(File &map)
    {
        clear();
        printw("+------------------------------------------+\n");
        printw("|                                          |\n");
        printw("|             Jogo Pausado!                |\n");
        printw("|                                          |\n");
        printw("+------------------------------------------+\n");
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
                start_new_game("maze.txt", 20, 24);
                break;
            case 2:
                load_game("saved_game.txt", 20, 24);
                break;
            case 3:
                map.save_file();
                pause_game(map);
                break;
            case 4:
                is_playing = false;
                printw("Saindo do jogo....");
                break;
            default:
                printw("Por favor, adicione uma seleção válida\n");
                break;
            }
        } while (input <= 0 || input >= 5);
    }

    void game(File map, Player player_1, Player player_2)
    {
        clock_t start;
        start = clock();
        float current_time = 0.0;

        while (is_playing)
        {
            erase();
            time(start, current_time);
            show_map(map, player_1, 1);
            show_map_transposed(map, player_2, 20);

            // napms(5000);
            win_game(current_time, player_1.get_view(), player_1, player_2);

            // nodelay(stdscr, TRUE);
            // TODO: See if this is the best way to handle both players inputs using kbhit()
            player_1.move(is_playing, map);
            if (!is_playing) // Cláusula de Guarda
                break;
            player_2.move(is_playing, map);
            // nodelay(stdscr, FALSE);
        }

        pause_game(map);
    }

    void time(clock_t start, float &current_time)
    {
        clock_t sum = 33.33;
        current_time = (clock() - start + sum) / (double)CLOCKS_PER_SEC;
        move(10, 30);
        printw("Clock: %f", current_time);
    }

    void win_game(int time, int view, Player player_1, Player player_2)
    {
        // TODO: Improve this win
        if (abs(player_1.get_x() - player_2.get_x()) < view && abs(player_1.get_y() - player_2.get_y()) < view) // Condição de Vitória
        {
            save_scoreboard(time);
            is_playing = false;
            clear();
            printw("+-----------------------------------------+\n");
            printw("|                                         |\n");
            printw("|  O tempo total foi: XXXX                |\n");
            printw("|                                         |\n");
            printw("+-----------------------------------------+\n");
            printw("\n");
            printw("+-----------------------------------------------------+\n");
            printw("| - Pressione 1 para iniciar um novo jogo             |\n");
            printw("| - Pressione 2 para continuar o jogo salvo           |\n");
            printw("| - Pressione 3 para Visualizar o Quadro de Pontuação |\n");
            printw("| - Pressione 4 para sair                             |\n");
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
                    start_new_game("maze.txt", 10, 10);
                    break;
                case 2:
                    load_game("saved_game.txt", 10, 10);
                    break;
                case 3:
                    show_scoreboard();
                    break;
                case 4:
                    is_playing = false;
                    printw("Saindo do jogo....");
                    break;
                default:
                    printw("Por favor, adicione uma seleção válida\n");
                    break;
                }
            } while (input <= 0 || input >= 5);
        }
    };

    void show_map(File file, Player &player, int win_x = 0, int win_y = 2)
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

    void show_map_transposed(File file, Player &player, int win_x = 0, int win_y = 2)
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

    void save_scoreboard(float current_time)
    {
        float *scores = new float[6];

        ifstream file;
        file.open("scoreboard.txt");
        string line;

        int counter = 0;
        while (getline(file, line))
        {
            if (counter == 4)
                break;

            scores[counter] = stof(line); // Função para converter String para Float
            counter++;
        }
        scores[5] = current_time;

        std::sort(scores, scores + 6);

        ofstream file_to_save;
        file_to_save.open("scoreboard.txt");
        file_to_save.clear();

        float score;
        for (int i = 0; i < 5; i++)
        {
            score = scores[i];
            file_to_save << score << endl;
        }

        file_to_save.close();
        delete[] scores;
    };

    void show_scoreboard()
    {
        clear();
        printw("+-------------------------------------+\n");
        printw("|                                     |\n");
        printw("|         Quadro de Pontuação         |\n");
        printw("|                                     |\n");
        printw("|         TOP 5 - Best Times          |\n");
        printw("|                                     |\n");
        printw("+-------------------------------------+\n");
        printw("\n");
        printw("+-------------------------------------+\n");
        printw("| - Pressione 1 para voltar           |\n");
        printw("+-------------------------------------+\n");
        printw("\n");

        ifstream file;
        file.open("scoreboard.txt");
        string line;

        int counter = 0;
        while (getline(file, line))
        {
            if (counter == 5)
                break;

            printw(line.c_str());
            printw("\n");
            counter++;
        }
        refresh();

        file.close();

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
    }
};

#endif // SYSTEM_H