/**
    PJC2 PROJEKT
    Menu.h
    Purpose: menu + gra kółko i krzyżyk

    @author Krzysztof Siwoń
    @version 0.9 24/10/16 
*/
#ifndef MENU_H
#define MENU_H
#include "curses.h"
#include <vector>
#include <string>

using namespace std;

class Menu
{
    public:
        WINDOW *window;// wskaznik na okno gry
        vector<string> menu_list;//nazwy w menu 
        vector<string> color_list;//menu kolorow
        int color;//kolor menu
        int highlight;//podswietlona pozycja menu
        int choice; //wybrana pozycja menu
        
        /**
         Konstruktor Menu

        @param WINDOW *okno
        @return okno menu
        */
        Menu(WINDOW *menu_win);// pobiera okno z konstruktora
        void draw_window();//Wyświetla okno menu.
        void menu_refresh(vector<string> list);//odswiezenie widoku menu
        void color_menu();//menu wyboru koloru
 
        /**
         Przesuwa kursor w menu

        @param int highlight: zwraca pozycje kursora
        */
        int menu_move(vector<string>list, int input);//interakcja menu
};

#endif /* MENU_H */

