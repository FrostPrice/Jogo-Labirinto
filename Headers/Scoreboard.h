#ifndef SCOREBOAD_H
#define SCOREBOAD_H

using namespace std;

#include <string>
#include <fstream>

#include "Player.h"

class Scoreboard
{
private:
    string name;
    int size;
    long double *scores;

public:
    // Methods
    void save_scoreboard(long double current_time)
    {
        ofstream scoreboard_file;
        scoreboard_file.open("scoreboard.txt", std::ios::out | std::ios::trunc);
        string line;

        int temp_size = size + 1;
        scores[size] = current_time;

        long double temp;
        for (int i = 0; i < temp_size; i++)
        {
            for (int j = i + 1; j < temp_size; j++)
            {
                if (scores[i] > scores[j])
                {
                    temp = scores[i];
                    scores[i] = scores[j];
                    scores[j] = temp;
                }
            }
        }

        long double current_score;
        for (int i = 0; i < size; i++)
        {
            current_score = scores[i];
            scoreboard_file << current_score << endl;
        }

        scoreboard_file.close();
    }

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

        ifstream scoreboard_file;
        scoreboard_file.open("scoreboard.txt");
        string line;

        int counter = 0;
        while (getline(scoreboard_file, line))
        {
            if (counter >= 5) // Para mostrar somente os 5 primeiros score, se existir mais linhas
                break;

            printw(line.c_str());
            printw("\n");
            counter++;
        }
        refresh();

        scoreboard_file.close();
    }

    // Setter
    void set_size(int size)
    {
        this->size = size;
    };

    void set_scores()
    {
        ifstream scoreboard_file;
        scoreboard_file.open("scoreboard.txt");
        string line;

        scores = new long double[size + 1];

        int counter_temp = 0;
        while (getline(scoreboard_file, line))
        {
            if (counter_temp > size)
                break;

            scores[counter_temp] = stold(line); // Função para converter String para Float
            counter_temp++;
        }

        scoreboard_file.close();
    }
};

#endif // SCOREBOAD_H