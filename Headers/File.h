#ifndef FILE_H
#define FILE_H

using namespace std;

#include <string>
#include <fstream>

class File
{
private:
    string name;
    int x, y;
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
                matrix[i][j] = map_char - 48;
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