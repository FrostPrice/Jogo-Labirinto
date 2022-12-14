#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

#include <iostream>

#include "Map.h"

class Player
{
private:
    string name;
    bool can_move;
    int x, y, view, last_position_value;
    int keyboard[4];

public:
    Player()
    {
        name = "Player X";
        can_move = true;
        x = 0;
        y = 0;
        view = 0;
        last_position_value = 0;
        keyboard[4] = {0};
    };

    // Methods
    void move(int input, bool &is_playing, bool &is_paused, Map &map)
    {
        int map_x = map.get_x();
        int map_y = map.get_y();
        int **map_matrix = map.get_matrix();

        if (can_move)
        {
            if (input == keyboard[0])
            {
                if (map_matrix[x - 1][y] == 0 || map_matrix[x - 1][y] == 3)
                {
                    last_position_value = map_matrix[x - 1][y];

                    map_matrix[x][y] = 0;
                    map_matrix[x - 1][y] = 2;
                    x += -1;
                }
            }
            else if (input == keyboard[1])
            {
                if (map_matrix[x + 1][y] == 0 || map_matrix[x + 1][y] == 3)
                {
                    last_position_value = map_matrix[x + 1][y];

                    map_matrix[x][y] = 0;
                    map_matrix[x + 1][y] = 2;
                    x += +1;
                }
            }
            else if (input == keyboard[2])
            {
                if (map_matrix[x][y - 1] == 0 || map_matrix[x][y - 1] == 3)
                {
                    last_position_value = map_matrix[x][y - 1];

                    map_matrix[x][y] = 0;
                    map_matrix[x][y - 1] = 2;
                    y += -1;
                }
            }
            else if (input == keyboard[3])
            {
                if (map_matrix[x][y + 1] == 0 || map_matrix[x][y + 1] == 3)
                {
                    last_position_value = map_matrix[x][y + 1];

                    map_matrix[x][y] = 0;
                    map_matrix[x][y + 1] = 2;
                    y += +1;
                }
            }
            else if (input == 112) // 112 == p
            {
                is_playing = false;
                is_paused = true;
            }
        }
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
    int get_last_position_value()
    {
        return this->last_position_value;
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
        this->x += x;
    }
    void set_y(int y)
    {
        this->y += y;
    }
    void set_view(int view)
    {
        this->view = view;
    }
    void set_last_position_value(int position)
    {
        this->last_position_value = position;
    }
    void set_can_move(bool can_move)
    {
        this->can_move = can_move;
    }
};

#endif // PLAYER_H