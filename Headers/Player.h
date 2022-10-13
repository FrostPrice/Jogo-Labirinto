#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    string name;
    int x, y, view;

public:
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
    void set_coord(int col, int lin)
    {
        x = col;
        y = lin;
    }
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