#ifndef PLAYER_H
#define PLAYER_H

#include "File.h"

using namespace std;

#include <iostream>

class Player
{
private:
    string name;
    int x, y, view;
    int keyboard[4];

public:
    // Methods
    void move(bool &is_playing, File &map)
    {
        int map_x = map.get_x();
        int map_y = map.get_y();
        int **map_matrix = map.get_matrix();

        nodelay(stdscr, TRUE);
        int input = getch();
        if (input == keyboard[0])
        {
            if (map_matrix[x - 1][y] == 0)
            {
                cout << "AAAAAAAAA";
                map_matrix[x][y] = 0;
                map_matrix[x - 1][y] = 2;
                x += -1;
            }
        }
        else if (input == keyboard[1])
        {
            if (map_matrix[x + 1][y] == 0)
            {
                map_matrix[x][y] = 0;
                map_matrix[x + 1][y] = 2;
                x += +1;
            }
        }
        else if (input == keyboard[2])
        {
            if (map_matrix[x][y - 1] == 0)
            {
                map_matrix[x][y] = 0;
                map_matrix[x][y - 1] = 2;
                y += -1;
            }
        }
        else if (input == keyboard[3])
        {
            if (map_matrix[x][y + 1] == 0)
            {
                map_matrix[x][y] = 0;
                map_matrix[x][y + 1] = 2;
                y += +1;
            }
        }
        else if (input == 112) // 112 == p
        {
            is_playing = false;
        }
        nodelay(stdscr, FALSE);
    }

    // Getters
    string get_name()
    {
        return this->name;
    }
    int get_x()
    {
        return this->x;
    }
    int get_y()
    {
        return this->y;
    }
    int get_view()
    {
        return this->view;
    }
    // Setters
    void set_coord(int *coords)
    {
        this->x = coords[0];
        this->y = coords[1];
    }
    void set_keyboard(int up, int down, int left, int right)
    {
        this->keyboard[0] = up;
        this->keyboard[1] = down;
        this->keyboard[2] = left;
        this->keyboard[3] = right;
    };
    void set_name(string name)
    {
        this->name = name;
    }
    void set_x(int x)
    {
        this->x = x;
    }
    void set_y(int y)
    {
        this->y = y;
    }
    void set_view(int view)
    {
        this->view = view;
    }
};

#endif // PLAYER_H