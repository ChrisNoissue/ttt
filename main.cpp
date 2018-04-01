/**
    PJC2 PROJEKT
    main.cpp
    Purpose: menu + gra kolko i krzyzyk

    @author Krzysztof Siwoń
    @version 0.9 24/10/16 
*/

#include <iostream>
#include "curses.h"
#include "Menu.h"
#include "Game.h"

int menu_choosing(Menu o_menuWin, Game o_gameWin, WINDOW *menu_window, WINDOW *game_window);

int main(void)
{
    WINDOW *menu_window, *game_window;	// Okna programu

    initscr();	// Rozpoczecie pracy z biblioteka CURSES
    curs_set(0);// Nie wyswietlaj kursora
    noecho();	// Nie wyswietlaj inputow z klawiatury
    cbreak();
    
    start_color(); // inicjalizacja kolorów
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    
    menu_window = newwin(LINES, 20, 0, COLS-20);// okno menu
    game_window = newwin(LINES, COLS-20, 0, 0);// okno gry
    
    Menu o_menuWin(menu_window);// tworzy objekt okna menu
    Game o_gameWin(game_window);// tworzy objekt okna gry
    
    menu_choosing(o_menuWin, o_gameWin, menu_window, game_window);// funkcja odpowiedzalna za sterowanie grą

    delwin(menu_window);// Usuniecie okien
    delwin(game_window);
    endwin();// Koniec pracy z CURSES
    return(0);
}


int menu_choosing(Menu o_menuWin, Game o_gameWin, WINDOW *menu_window, WINDOW *game_window)
{
    int input;
    do 
    {
        wbkgdset(menu_window, COLOR_PAIR(o_menuWin.color)); //odpowiada za kolor okna menu
        wbkgdset(game_window, COLOR_PAIR(o_menuWin.color)); //odpowiada za kolor okna gry
        wclear(menu_window);
        o_menuWin.draw_window(); // rysuje okno menu
        o_gameWin.draw_window(); // rysuje okno gry
        mvwprintw(game_window, LINES-3, COLS-70, "Poruszanie sie po menu: \"gora\", \"dol\"");
        mvwprintw(game_window, LINES-2, COLS-70, "Zatwierdz wybor: \"ENTER\"");
        mvwprintw(menu_window, LINES-(o_menuWin.color_list.size()+2), 1, "ZMIEN KOLOR");
        for(int i=0;i<=o_menuWin.color_list.size()-1;i++)
            mvwprintw(menu_window,LINES-(o_menuWin.color_list.size()+1)+i, 1,"F%d - %s", i+1,  o_menuWin.color_list[i].c_str());
        wrefresh(game_window);
        wrefresh(menu_window);
        o_menuWin.menu_refresh(o_menuWin.menu_list); // odswierzenie menu
        o_menuWin.choice = 0; // aktualna pozycja w menu
        
        input = wgetch(menu_window); // Oczekiwanie na klawisz
        for(int i=1;i<=o_menuWin.color_list.size();i++) if(input == KEY_F(i)) // sprawdza na bieżąco i zmienia kolory w menu
        {
            o_menuWin.color = i;
            wclear(game_window);
        }
        o_menuWin.menu_move(o_menuWin.menu_list, input);// jesli wcisnieto strzalke zmien pozycje menu
        o_menuWin.menu_refresh(o_menuWin.menu_list); // odswierzenie menu

        if(input == 10) o_menuWin.choice = o_menuWin.highlight; // po zatwierdzeniu enterem przypisuje nr pozycji menu do choice

        switch(o_menuWin.choice)
        {
            case 1: // nowa gra
                o_gameWin.run();
                break;
            case 2: // zasady
                o_gameWin.rules();
                break;
            case 3: // historia
                o_gameWin.history();
                break;
            case 4: // autorzy
                o_gameWin.authors();
                break;
            case 5: // wyjscie
                return 0;
        }
        wrefresh(game_window);
	} while(input != 27); // petla menu działa dopóki nie wcisnięto "ESC" 
} 