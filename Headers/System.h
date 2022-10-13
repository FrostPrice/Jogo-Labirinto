#ifndef SYSTEM_H
#define SYSTEM_H

using namespace std;

#include <iostream>
#include <curses.h>
#include <ncurses.h>

#include "File.h"
#include "Player.h"

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
        cout << "+--------------------------------------+\n";
        cout << "|            Welcome to the            |\n";
        cout << "|                                      |\n";
        cout << "|     Fehl Sky Maze - Lost in the      |\n";
        cout << "|         Fractured But Whole          |\n";
        cout << "|                                      |\n";
        cout << "+--------------------------------------+\n";
        cout << endl;
        cout << "+--------------------------------------+\n";
        cout << "| - Press 1 to Start a New Game        |\n";
        cout << "| - Press 2 to Continue Saved Game     |\n";
        cout << "| - Press 3 to Save                    |\n";
        cout << "| - Press 4 to Quit                    |\n";
        cout << "+--------------------------------------+\n";
        cout << endl;

        int input;
        do
        {
            cin >> input;
            switch (input)
            {
            case 1:
                start_new_game("labirinto.txt", 10, 10);
                break;
            case 2:
                // TODO
                // load_game();
                break;
            case 3:
                // TODO
                // save_game();
                break;
            case 4:
                is_playing = false;
                cout << "Exiting game....";
                break;
            default:
                cout << "Please, add a valid selection" << endl;
                break;
            }
        } while (input <= 0 || input >= 5);
    }

    void start_new_game(string name, int size_x, int size_y)
    {
        Player player_1;
        player_1.set_name("Player 1");
        player_1.set_coord(3, 4);
        player_1.set_view(3);

        File map;
        map.set_name(name);
        map.set_size(size_x, size_y);
        map.load_file();

        game(map, player_1);
    }

    void game(File map, Player player_1)
    {
        initscr();            // Start Curses
        curs_set(0);          // Make the cursor invisible
        keypad(stdscr, TRUE); // Enable the use of the Arrow Key
        noecho();             // Don't show what the user types

        while (is_playing)
        {
            clear();
            show_map(map, player_1);
            show_map(map, player_1, 20);
            move_player(player_1, map);

            refresh();
        }

        endwin(); // End Curses
    }

    void show_map(File file, Player player, int win_x = 0, int win_y = 2)
    {
        int file_x = file.get_x();
        int file_y = file.get_y();
        int **file_matrix = file.get_matrix();
        int player_x = player.get_x();
        int player_y = player.get_y();
        int player_view = player.get_view();

        mvprintw(0, win_x + 1, player.get_name().c_str()); // Uses c_str to convert string to char* and mvprint to show in the terminal a sequence of char
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
                        mvaddch(win_y, temp_win_x, ' '); // Move the cursor to the defined x and y, and add a char to that position
                        break;
                    case 1:
                        mvaddch(win_y, temp_win_x, char(219));
                        break;
                    case 2:
                        mvaddch(win_y, temp_win_x, char(1));
                        break;
                    }
                }
                temp_win_x++; // Used to make the player view fixed on the side of the screen
            }
            cout << endl;
            win_y++; // Used to make the player view fixed on the side of the screen
        }
        cout << endl;
    }

    void move_player(Player &player, File &map)
    {
        int map_x = map.get_x();
        int map_y = map.get_y();
        int **map_matrix = map.get_matrix();
        int player_x = player.get_x();
        int player_y = player.get_y();

        char input = getch();
        switch (input)
        {
        case 'w':
            if (map_matrix[player_x - 1][player_y] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x - 1][player_y] = 2;
                player.set_x(player_x - 1);
            }
            break;
        case 's':
            if (map_matrix[player_x + 1][player_y] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x + 1][player_y] = 2;
                player.set_x(player_x + 1);
            }
            break;
        case 'a':
            if (map_matrix[player_x][player_y - 1] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x][player_y - 1] = 2;
                player.set_y(player_y - 1);
            }
            break;
        case 'd':
            if (map_matrix[player_x][player_y + 1] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x][player_y + 1] = 2;
                player.set_y(player_y + 1);
            }
            break;
        case 'p':
            // TODO: Send to the main_menu
            break;
        }

        // int input = getch();
        // switch (input)
        // {
        // case KEY_UP:
        //     if (map_matrix[player_x - 1][player_y] == 0)
        //     {
        //         map_matrix[player_x][player_y] = 0;
        //         map_matrix[player_x - 1][player_y] = 2;
        //         player.set_x(player_x - 1);
        //     }
        //     break;
        // case KEY_DOWN:
        //     if (map_matrix[player_x + 1][player_y] == 0)
        //     {
        //         map_matrix[player_x][player_y] = 0;
        //         map_matrix[player_x + 1][player_y] = 2;
        //         player.set_x(player_x + 1);
        //     }
        //     break;
        // case KEY_LEFT:
        //     if (map_matrix[player_x][player_y - 1] == 0)
        //     {
        //         map_matrix[player_x][player_y] = 0;
        //         map_matrix[player_x][player_y - 1] = 2;
        //         player.set_y(player_y - 1);
        //     }
        //     break;
        // case KEY_RIGHT:
        //     if (map_matrix[player_x][player_y + 1] == 0)
        //     {
        //         map_matrix[player_x][player_y] = 0;
        //         map_matrix[player_x][player_y + 1] = 2;
        //         player.set_y(player_y + 1);
        //     }
        //     break;
        // case 'p':
        //     // TODO: Send to the main_menu
        //     break;
        // }
    }
};

#endif // SYSTEM_H