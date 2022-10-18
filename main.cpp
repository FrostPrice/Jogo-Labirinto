/* Mateus Barbosa, Matheus De Oliveira Rocha, Eduardo Augusto Formigheri Moretto */

#pragma region TODOS
/*
--------------------------------------------------------------------------------------------
// - Faça um programa que lê um arquivo com indicações de espaço livre(0), barreira(1), personagem(2) - OK
// - O jogo deve ter uma função que permite o movimento do personagem pelas posições livres sem deixar rastro - OK
// - Modifiquem o método de movimentação para o jogador ficar fixo numa posição da tela e o mapa se mover para executar o movimento - OK
// - Criem um campo de visão para indicar quanto do mapa vai aparecer. se o valor for 3, deve aparecer somente 3 - OK
// - Faça um menu com as opções:
    - Novo jogo - OK
    - Continuar - OK
    - Salvar - OK
// - Faça o mapa do jogador 2 ser exibido de forma transposta em relação ao do jogador 1 - OK
// - Faça o jogador 1 vencer quando o jogador 2 estiver perto (dentro do campo de visão do jogador 1) - OK
// - Conte o tempo que levou para isso acontecer (tempo de fim - tempo de inicio do jogo) - OK
// - Inclua um ranking no menu com os melhores tempos ordenados - OK
// - Faça portais que permitem o jogador aparecer em uma posição aleatória livre no mapa. - OK
// - Faça itens que quando pegos reduzem o campo de visão do adversário por 5 segundos. - OK
// - Faça um item que quando pego bloqueia o movimento do adversário por 5 segundos. - OK
--------------------------------------------------------------------------------------------
*/
#pragma endregion TODOS

using namespace std;

#include <locale.h>
#include <time.h>
#include <curses.h>
#include <ncurses.h>

#include "./Headers/System.h"

int main()
{
    srand(time(NULL));

    initscr();            // Start Curses
    curs_set(0);          // Make the cursor invisible
    cbreak();             // Desabilitar buffer de linha
    noecho();             // Não mostrar o que o usuário digita
    keypad(stdscr, TRUE); // Habilitar o uso da tecla de seta

    System system;
    system.__init__();

    endwin(); // End Curses

    return 0;
}