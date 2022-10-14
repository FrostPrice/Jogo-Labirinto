#ifndef FILE_H
#define FILE_H

using namespace std;

#include <string>
#include <fstream>

#include "Player.h"

class File
{
private:
    string name;
    int x, y;
    int **players_coords;
    int **matrix;

public:
    // Methods
    void load_file()
    {
        matrix = new int *[x];
        for (int i = 0; i < x; i++)
        {
            matrix[i] = new int[y];
        }

        ifstream map;
        map.open(name);
        char map_char;

        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                map >> map_char;
                matrix[i][j] = map_char - 48; // The values for char start at 48
            }
        }
    }

    void save_file()
    {
        ofstream map;
        map.open("saved_game.txt");
        char map_char;

        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                map_char = matrix[i][j] + 48;
                map << map_char;
            }
            map << endl;
        }
    }

    void define_players_coords(int num_players)
    {
        players_coords = new int *[num_players];
        for (int i = 0; i < 2; i++)
        {
            players_coords[i] = new int[2]; // For x and y coords
        }

        int temp_i = 0;
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                if (matrix[i][j] == 2)
                {
                    players_coords[temp_i][0] = i; // x
                    players_coords[temp_i][1] = j; // y
                    temp_i++;
                }
            }
        }
    }

    // Getters
    int get_x()
    {
        return this->x;
    }
    int get_y()
    {
        return this->y;
    }
    int **get_players_coords()
    {
        return this->players_coords;
    };
    int **get_matrix()
    {
        return this->matrix;
    }
    // Setters
    void set_size(int col, int lin)
    {
        x = col;
        y = lin;
    }
    void set_name(string name)
    {
        this->name = name;
    }
};

#endif // FILE_H