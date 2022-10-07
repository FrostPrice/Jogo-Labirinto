#pragma region TODOS
// - Faça um programa que lê um arquivo com indicações de espaço livre(0), barreira(1), personagem(2) - OK
// - O jogo deve ter uma função que permite o movimento do personagem pelas posições livres sem deixar rastro - OK
// - Modifiquem o método de movimentação para o jogador ficar fixo numa posição da tela e o mapa se mover para executar o movimento
// - Criem um campo de visão para indicar quanto do mapa vai aparecer. se o valor for 3, deve aparecer somente 3
#pragma endregion TODOS

using namespace std;

#include <iostream>
#include <string>
#include <fstream>
#include <curses.h>
#include <ncurses.h>

struct Player
{
    int x, y, view;
    int *view_adjacent_values;

    void
    coord(int col, int lin)
    {
        x = col;
        y = lin;
    }

    void set_view(int view_value)
    {
        view = view_value;
    }

    void create_view_adjacent_values()
    {
        int temp = view;
        view_adjacent_values = new int[view * 2 + 1];
        for (int i = 0; i < view * 2 + 1; i++)
        {
            view_adjacent_values[i] = temp;
            temp--;
        }
    }
};

struct File
{
    string name;
    int x, y;
    int **matrix;

    void size(int col, int lin)
    {
        x = col;
        y = lin;
    }

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
                matrix[i][j] = map_char - 48; // Porque menos 48?
            }
        }
    }
};

void show_map(File file, Player player)
{
    move(0, 0); // Move the cursor to the begining of the Terminal

    // for (int i = 0; i < file.x; i++)
    // {
    //     for (int j = 0; j < file.y; j++)
    //     {
    //         switch (file.matrix[i][j])
    //         {
    //         case 0:
    //             mvprintw(i, j, " ");
    //             break;
    //         case 1:
    //             mvprintw(i, j, "1");
    //             break;
    //         case 2:
    //             mvprintw(i, j, "2");
    //             break;
    //         }
    //     }
    //     cout << endl;
    // }

    // cout << endl;

    // TODO: Fix this fucking thing
    for (int i = player.view * 2 + 1; i >= 0; i--)
    {
        for (int j = player.view * 2 + 1; j >= 0; j--)
        {
            if (player.x + player.view_adjacent_values[i] >= 0 && player.y + player.view_adjacent_values[j] >= 0 && player.x + player.view_adjacent_values[i] < player.view * 2 + 1 && player.y + player.view_adjacent_values[j] < player.view * 2 + 1)
            {
                switch (file.matrix[player.x + player.view_adjacent_values[i]][player.y + player.view_adjacent_values[j]])
                {
                case 0:
                    mvprintw(i, j, " ");
                    break;
                case 1:
                    mvprintw(i, j, "1");
                    break;
                case 2:
                    mvprintw(i, j, "2");
                    break;
                }
            }
        }
        cout << endl;
    }
    cout << endl;
};

void move_player(Player &player, File &map)
{
    char input = getch();
    switch (input)
    {
    case 'w':
    case 'W':
        if (map.matrix[player.x - 1][player.y] == 0)
        {
            map.matrix[player.x][player.y] = 0;
            map.matrix[player.x - 1][player.y] = 2;
            player.x = player.x - 1;
        }
        break;
    case 's':
    case 'S':
        if (map.matrix[player.x + 1][player.y] == 0)
        {
            map.matrix[player.x][player.y] = 0;
            map.matrix[player.x + 1][player.y] = 2;
            player.x = player.x + 1;
        }
        break;
    case 'a':
    case 'A':
        if (map.matrix[player.x][player.y - 1] == 0)
        {
            map.matrix[player.x][player.y] = 0;
            map.matrix[player.x][player.y - 1] = 2;
            player.y = player.y - 1;
        }
        break;
    case 'd':
    case 'D':
        if (map.matrix[player.x][player.y + 1] == 0)
        {
            map.matrix[player.x][player.y] = 0;
            map.matrix[player.x][player.y + 1] = 2;
            player.y = player.y + 1;
        }
        break;
    }
};

int main()
{
    initscr();   // Start Curses
    curs_set(0); // Make the cursor invisible
    noecho();    // Don't show what the user types
    clear();     // Don't ask me

    Player player_1;
    player_1.coord(3, 4);
    player_1.set_view(3);
    player_1.create_view_adjacent_values();

    File map;
    map.name = "labirinto.txt";
    map.size(10, 10);
    map.load_file();

    while (true)
    {
        clear();
        show_map(map, player_1);
        move_player(player_1, map);
        refresh();
    }

    endwin(); // End Curses

    return 0;
}