/**
    PJC2 PROJEKT
    Game.h
    Purpose: menu + gra kółko i krzyżyk

    @author Krzysztof Siwoń
    @version 0.9 24/10/16 
*/
#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <vector>
#include "curses.h"
#include <time.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
	

using namespace std;

class Game 
{
public:
    WINDOW *window; //okno w ktorym wyswietlana jest gra
    int highlight; //pozycja podswietlenia w menu
    int moves; //ilosc ruchow na planszy (max 9)
    int player_symbol; //pionek gracza
    int human_players;//ilosc graczy
    int board_size; //wielkosc planszy gry
    int score; // wynik gry
    int history_count; // ilosc rozegranych gier
    vector<char> charBoard; //tablica gry
    vector<char> XorY; // gracz x albo y przy zmianie graczy na planszy
    vector<char> vs;
    vector<string> history_bufer; // bufor historii partii
    
    /**
         Konstruktor okna Game
     * 
        @param WINDOW *game_window
        @return okno game
        */
    Game(WINDOW *game_window);//pobiera okno z konstruktora
    void draw_window(); // rysuje okno
    void init_board(); //inicjalizuje plansze
    int choose_player(); // wybor pionka
    int set_human_players(); // ustawienie ile graczy ludzkich
    void draw_board(); //rysuje tablice gry
    int play(); // mechanika gry
    void game_refresh();//odswiezenie planszy gry
    int check_win(); // sprawdza wygrana
    int game_move(vector<char>* list, int input);//zmiana pozycji na planszy
    void rules(); //wyswietla zasady gry
    void authors(); // wyswietla autorow
    void run(); // rozgrywka
    void to_history(char winner); // zapisuje date zakonczonej partii
    void history();// wyswietla historie partii 
    const string current_date_time(); // zwraca aktualna date
    void to_file(string buffer);// zapisuje do pliku historie partii
    void from_file();// odczytuje z pliku historie partii
    int get_free_field();// zwraca losowe wolne pole z tablicy gry
};

#endif /* GAME_H */

