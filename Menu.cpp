/**
 *  PJC2 PROJEKT
    Menu.cpp
    Purpose: menu + gra kółko i krzyżyk

    @author Krzysztof Siwoń
    @version 0.9 24/10/16 
*/
#include <iostream>
#include "Menu.h"
#include <vector>
#include <string>
using namespace std;

Menu::Menu(WINDOW *menu_win)
{
    color = 0;
    window = menu_win;
    choice = 0;
    highlight = 1;
    
    menu_list.push_back("Nowa gra");
    menu_list.push_back("Zasady gry");
    menu_list.push_back("Historia");
    //menu_list.push_back("Zmien kolory"); // pozycja w menu 
    menu_list.push_back("Autorzy");
    menu_list.push_back("Wyjscie");
    
    color_list.push_back("Bialy");
    color_list.push_back("Czerwony");
    color_list.push_back("Zielony");
    color_list.push_back("Magenta");
    color_list.push_back("Cyjan");
    color_list.push_back("Zolty");
    color_list.push_back("Cz/B");
}

void Menu::color_menu() 
{
    int input;
    draw_window();
    mvwprintw(window, 1, 3, "ZMIANA KOLORU");
    wrefresh(window);
    do 
    {
        menu_refresh(color_list);
        input = wgetch(window);		// Oczekiwanie na klawisz
        
        menu_move(color_list, input);
        menu_refresh(color_list);
        wrefresh(window);
        if(input == 10) 
        {
            color = highlight;
            break;
        }
    } while(input != 27);
}


void Menu::menu_refresh(vector<string> list)
{
    for(int i = 0; i < list.size(); ++i)
    {
        if( highlight == i+1) 
        {
            wattron( window, A_STANDOUT );
            mvwprintw(window, 3+i, 2, list[i].c_str());
            wattroff( window, A_STANDOUT );
        }
        else mvwprintw(window, 3+i, 2, list[i].c_str());
    }
    wrefresh(window);
}

void Menu::draw_window()
{
    box(window, 0, 0);			// Standardowe ramki
    mvwprintw(window, 1, 7, "MENU");
    keypad(window, 1);
    wrefresh(window); 
}

int Menu::menu_move(vector<string> list, int input)
{
    if(input == KEY_UP)
    {
        highlight--;
        highlight = (highlight<1) ? list.size() : highlight;// zapetlenie kursora w menu
        return highlight;
    }
    else if(input == KEY_DOWN)
    {
        highlight++;
        highlight = (highlight>list.size()) ? 1 : highlight;// zapetlenie kursora w menu
        return highlight;
    }     
}
