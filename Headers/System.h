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
    void start_game()
    {
        initscr();   // Start Curses
        curs_set(0); // Make the cursor invisible
        noecho();    // Don't show what the user types
        clear();     // Don't ask me

        Player player_1;
        player_1.set_coord(3, 4);
        player_1.set_view(3);

        File map;
        map.set_name("labirinto.txt");
        map.set_size(10, 10);
        map.load_file();

        while (is_playing)
        {
            clear();
            show_map(map, player_1);
            move_player(player_1, map);
            refresh();
        }

        endwin(); // End Curses
    };
    ;
    void show_map(File file, Player player)
    {

        {
            int file_x = file.get_x();
            int file_y = file.get_y();
            int **file_matrix = file.get_matrix();
            int player_x = player.get_x();
            int player_y = player.get_y();
            int player_view = player.get_view();

            int win_x = 0; // Used to make the player view fixed on the side of the screen
            for (int i = player_x - player_view; i < player_x + player_view; i++)
            {
                int win_y = 0; // Used to make the player view fixed on the side of the screen
                for (int j = player_y - player_view; j < player_y + player_view; j++)
                {
                    if (i >= 0 && j >= 0 && i < file_x && j < file_y)
                    {
                        switch (file_matrix[i][j])
                        {
                        case 0:
                            mvaddch(win_x, win_y, ' '); // Move the cursor to the defined x and y, and add a char to that position
                            break;
                        case 1:
                            mvaddch(win_x, win_y, char(219));
                            break;
                        case 2:
                            mvaddch(win_x, win_y, char(1));
                            break;
                        }
                    }
                    win_y++;
                }
                cout << endl;
                win_x++;
            }
            cout << endl;
        };
    };
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
        case 'W':
            if (map_matrix[player_x - 1][player_y] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x - 1][player_y] = 2;
                player.set_x(player_x - 1);
            }
            break;
        case 's':
        case 'S':
            if (map_matrix[player_x + 1][player_y] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x + 1][player_y] = 2;
                player.set_x(player_x + 1);
            }
            break;
        case 'a':
        case 'A':
            if (map_matrix[player_x][player_y - 1] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x][player_y - 1] = 2;
                player.set_y(player_y - 1);
            }
            break;
        case 'd':
        case 'D':
            if (map_matrix[player_x][player_y + 1] == 0)
            {
                map_matrix[player_x][player_y] = 0;
                map_matrix[player_x][player_y + 1] = 2;
                player.set_y(player_y + 1);
            }
            break;
        }
    };
};

#endif // SYSTEM_H