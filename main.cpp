#pragma region TODOS
// - Faça um programa que lê um arquivo com indicações de espaço livre(0), barreira(1), personagem(2) - OK
// - O jogo deve ter uma função que permite o movimento do personagem pelas posições livres sem deixar rastro - OK
// - Modifiquem o método de movimentação para o jogador ficar fixo numa posição da tela e o mapa se mover para executar o movimento - OK
// - Criem um campo de visão para indicar quanto do mapa vai aparecer. se o valor for 3, deve aparecer somente 3 - OK
// Faça um menu com as opções:
//      - Novo jogo
//      - Continuar
//      - Salvar
// - Faça o mapa do jogador 2 ser exibido de forma transposta em relação ao do jogador 1
// - Faça o jogador 1 vencer quando o jogador 2 estiver perto (dentro do campo de visão do jogador 1)
// - Conte o tempo que levou para isso acontecer (tempo de fim - tempo de inicio do jogo)
// - Inclua um ranking no menu com os melhores tempos ordenados
// - Faça portais que permitem o jogador aparecer em uma posição aleatória livre no mapa
// - Faça itens que quando pegos reduzem o campo de visão do adversário por 5 segundos.
// - Faça um item que quando pego bloqueia o movimento do adversário por 5 segundos.
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

    void coord(int col, int lin)
    {
        x = col;
        y = lin;
    }

    void set_view(int view_value)
    {
        view = view_value;
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
                matrix[i][j] = map_char - 48;
            }
        }
    }
};

void show_map(File file, Player player)
{
    int win_x = 0; // Used to make the player view fixed on the side of the screen
    for (int i = player.x - player.view; i < player.x + player.view; i++)
    {
        int win_y = 0; // Used to make the player view fixed on the side of the screen
        for (int j = player.y - player.view; j < player.y + player.view; j++)
        {
            if (i >= 0 && j >= 0 && i < file.x && j < file.y)
            {
                switch (file.matrix[i][j])
                {
                case 0:
                    mvaddch(win_x, win_y, ' ');
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