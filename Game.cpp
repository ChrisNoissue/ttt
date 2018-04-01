/**
    PJC2 PROJEKT
    Game.cpp
    Purpose: menu + gra kółko i krzyżyk

    @author Krzysztof Siwoń
    @version 0.9 24/10/16 
*/
#include "Game.h"

Game::Game(WINDOW *game_window) 
{
    window = game_window;
    board_size = 9;
    XorY.push_back('X');
    XorY.push_back('O');
    vs.push_back('1');
    vs.push_back('2');
    history_bufer.clear();
    history_count = 0;
}

void Game::run()
{
    if(choose_player() && set_human_players())
    {
        wclear(window);
        draw_window();
        do draw_board(); 
        while(play());
    }
    wrefresh(window);
}

void Game::draw_window()
{
    box(window, 0, 0);
    keypad(window, 1);
    wrefresh(window);
}

int Game::choose_player() 
{
    wclear(window);
    draw_window();
    highlight = 1;
    mvwprintw(window, 2, 2, "Wybierz swoj symbol: ");
    mvwaddch(window, 4, 12, 'X');
    mvwaddch(window, 4, 8, 'O');
    int input = 0;
    do
    {
        input = wgetch(window);
        game_move(&XorY, input);
        for(int i = 0; i < 2; ++i)
        {
            if( highlight == i+1) 
            {
                wattron( window, A_STANDOUT );
                mvwaddch(window, 4, 12-i*4, XorY[i]);
                wattroff( window, A_STANDOUT );
            }
            else mvwaddch(window, 4, 12-i*4, XorY[i]);
        }
    }
    while (input != 10 && input != 27);
    {
        if(input == 10) 
        {
            if(highlight == 2) player_symbol = 0; // X
            else if(highlight = 1) player_symbol = 1; // O
            return 1;
        }
        else 
            return 0;
    }
    
}

int Game::set_human_players()
{
    wclear(window);
    draw_window();
    highlight = 1;
    mvwprintw(window, 2, 2, "Ilu graczy realnych");
    mvwaddch(window, 4, 12, '1');
    mvwprintw(window, 4, 14, "(Gra z komputerem)");
    mvwaddch(window, 4, 8, '2');

    int input = 0;
    do
    {
        input = wgetch(window);
        game_move(&vs, input);
        for(int i = 0; i < 2; ++i)
        {
            if( highlight == i+1) 
            {
                wattron( window, A_STANDOUT );
                mvwaddch(window, 4, 12-i*4, vs[i]);
                wattroff( window, A_STANDOUT );
                mvwprintw(window, 4, 14, "(Gra z komputerem)");
            }
            else mvwaddch(window, 4, 12-i*4, vs[i]);
        }
    }
    while (input != 10 && input != 27);
    {
        if(input == 10) 
        {
            if(highlight == 2) human_players = 0; // 2 graczy
            else if(highlight = 1) human_players = 1; // 1 gracz
            return 1;
        }
        else 
            return 0;
    }    
    
}

void Game::init_board()
{
    charBoard.clear();
    for(int i=1; i<=board_size;i++) charBoard.push_back('_');
}

void Game::draw_board()
{
    wclear(window);
    draw_window();
    for(int i=2;i<=8;i++) 
    {
        mvwaddch(window, i, 4, '|');
        mvwaddch(window, i, 8, '|');
        mvwaddch(window, i, 12, '|');
        mvwaddch(window, i, 8, '|');
        mvwaddch(window, i, 12, '|');
        mvwaddch(window, i, 16,'|');

        if(i%2 == 0)
        {
            for(int h=4;h<=16;h++) mvwaddch(window, i, h, '-');
        }
    }
    mvwprintw(window, LINES-3, COLS-70, "w celu wyjscia z gry wcisnij: \"ESC\"");
    wrefresh(window);
}

int Game::play() 
{
    int input;
    score = 0;
    int moves = 0;
    int choice = 0;
    //init_board();
    
    /* WSPOLRZEDNE KAZDEJ KRATKI
       (3,6)(3,10)(3,14)
       (5,6)(5,10)(5,14)
       (7,6)(7,10)(7,14)*/
    init_board();
    mvwprintw(window, 0, (COLS-20)/2,"NOWA GRA");
    
    do 
    {
        game_refresh();
        mvwprintw(window, 12, 5,"Ruch Gracza \"%c\"", XorY[!player_symbol]);
        input = wgetch(window);		// Oczekiwanie na klawisz
        game_move(&charBoard, input);
        wrefresh(window);
        if(input == 10) 
        {
            mvwprintw(window, 12, 5,"Ruch Gracza \"%c\"", XorY[player_symbol]);
            choice = highlight; // po zatwierdzeniu enterem przypisuje nr pozycji menu do choice
            if(charBoard[choice-1] == '_')
            {
                if(player_symbol == 1) player_symbol = 0;
                else player_symbol = 1;
                charBoard[choice-1] = XorY[player_symbol];
                moves++;
            }
            if(human_players == 1)
            {
                usleep(100000);
                int ai_move = get_free_field();
                if(player_symbol == 1) player_symbol = 0;
                else player_symbol = 1;
                charBoard[ai_move] = XorY[player_symbol];
                moves++;
            }
            score = check_win();
        }
        wrefresh(window);
    }
    while(input != 27 && moves != 9 && score!=1 && score!=2 && score!=3);
        
    if((score != 2 && score !=1) && moves ==9) score = 3;
    switch(score)
    {
        case 1:
            draw_board();
            game_refresh();
            mvwprintw(window, 12, 5,"Wygral gracz \"X\"");
            to_history('X');
            break;
        case 2:
            draw_board();
            game_refresh();
            mvwprintw(window, 12, 5,"Wygral gracz \"O\"");
            to_history('O');
            break;
        case 3:
            draw_board();
            game_refresh();
            mvwprintw(window, 12, 5,"###### Remis #######");
            to_history('=');
            break;
    }
    wrefresh(window);
}

void Game::game_refresh()
{
    for(int i = 0; i < board_size; ++i)
    {
        if(i<3)
        {
            if(highlight == i+1)
            {
                wattron( window, A_STANDOUT );
                mvwaddch(window, 3, 6+i*4, charBoard[i]);
                wattroff( window, A_STANDOUT );
            }
            else mvwaddch(window, 3, 6+i*4, charBoard[i]);
        }
        else if(i>2 && i <6)
        {
            if(highlight == i+1)
            {
                wattron( window, A_STANDOUT );
                mvwaddch(window, 5, 6+(i-3)*4, charBoard[i]);
                wattroff( window, A_STANDOUT );
            }
            else mvwaddch(window, 5, 6+(i-3)*4, charBoard[i]);
        }
        else if(i>5 && i<9)
        {
            if(highlight == i+1)
            {
                wattron( window, A_STANDOUT );
                mvwaddch(window, 7, 6+(i-6)*4, charBoard[i]);
                wattroff( window, A_STANDOUT );
            }
            else mvwaddch(window, 7, 6+(i-6)*4, charBoard[i]);
        }
    }
    wrefresh(window);
}

int Game::check_win()
{
    if(        charBoard[0] == 'X' && charBoard[1] == 'X' && charBoard[2] == 'X' 
            || charBoard[3] == 'X' && charBoard[4] == 'X' && charBoard[5] == 'X'
            || charBoard[6] == 'X' && charBoard[7] == 'X' && charBoard[8] == 'X'
            || charBoard[0] == 'X' && charBoard[3] == 'X' && charBoard[6] == 'X'
            || charBoard[1] == 'X' && charBoard[4] == 'X' && charBoard[7] == 'X'
            || charBoard[2] == 'X' && charBoard[5] == 'X' && charBoard[8] == 'X'
            || charBoard[0] == 'X' && charBoard[4] == 'X' && charBoard[8] == 'X'
            || charBoard[6] == 'X' && charBoard[4] == 'X' && charBoard[2] == 'X')
    {
        score = 1;
        return score;
    }
    else if(   charBoard[0] == 'O' && charBoard[1] == 'O' && charBoard[2] == 'O' 
            || charBoard[3] == 'O' && charBoard[4] == 'O' && charBoard[5] == 'O'
            || charBoard[6] == 'O' && charBoard[7] == 'O' && charBoard[8] == 'O'
            || charBoard[0] == 'O' && charBoard[3] == 'O' && charBoard[6] == 'O'
            || charBoard[1] == 'O' && charBoard[4] == 'O' && charBoard[7] == 'O'
            || charBoard[2] == 'O' && charBoard[5] == 'O' && charBoard[8] == 'O'
            || charBoard[0] == 'O' && charBoard[4] == 'O' && charBoard[8] == 'O'
            || charBoard[6] == 'O' && charBoard[4] == 'O' && charBoard[2] == 'O')
    {
        score = 2;
        return score;
    }
}

int Game::game_move(vector<char>* list, int input)
{
        if(input == KEY_UP)
        {
            highlight -= 3;
            if(highlight == 0)highlight = 9;
            else if(highlight == -1) highlight = 8;
            else if(highlight == -2) highlight = 7;
            return highlight;   
        }
        else if(input == KEY_DOWN)
        {
            if(highlight == 9) highlight = 3;
            else highlight = (highlight > board_size-3) ? highlight%3 : highlight += 3;
            return highlight;
        }
        else if(input == KEY_LEFT)
        {
            highlight--;
            highlight = (highlight < 1) ? list->size() : highlight;
            return highlight;
        }
        else if(input == KEY_RIGHT)
        {
            highlight++;
            highlight = (highlight > list->size()) ? 1 : highlight;
            return highlight;
        }
}

void Game::rules()
{
    wclear(window);
    mvwprintw(window, 1, 1,"-- ZASADY GRY --");
    mvwprintw(window, 2, 1,"Gracze stawiaja na przemian kolko i krzyzyk");
    mvwprintw(window, 3, 1,"dazac do zajecia trzech pol w jednej linii.");
    mvwprintw(window, 4, 1,"Wygrywa ten z graczy, ktoremu jako pierwszemu ");
    mvwprintw(window, 5, 1,"uda ulozyc sie trzy znaki w jednej linii.");
}

void Game::authors()
{
    wclear(window);
    mvwprintw(window, 1, 1,"-- AUTORZY --");
    mvwprintw(window, 2, 1,"Krzysztof Siwon");
    mvwprintw(window, 3, 1,"Wykonano na: Programowanie w jezyku C2");
}

void Game::to_history(char winner)
{
    string buffer;
    buffer = current_date_time();
    buffer.push_back(winner);
    to_file(buffer);
    
    history_bufer.push_back(buffer);
    history_count++;
}

void Game::history()
{
    wclear(window);
    mvwprintw(window, 1, 1,"-- HISTORIA GIER --");
    mvwprintw(window, 15, 1,"oznaczenia:");
    mvwprintw(window, 16, 1,"\"=\" - remis");
    mvwprintw(window, 17, 1,"\"X\" - wygral X");
    mvwprintw(window, 18, 1,"\"O\" - wygral O");
    mvwprintw(window, 3, 3,"lp.\tdata\t\tgodz.\t\twygral");
    
    if(history_count == 0)
        from_file();
    
    if(history_bufer.size() > 10)
    {
        int j = 0;
        for(int i=history_bufer.size()-10;i<history_bufer.size();i++)
        {
            usleep(100000);
            mvwprintw(window, 13-j, 3,"%d:\t%s",i+1, history_bufer[i].c_str());
            j++;
            wrefresh(window);
        }
    }
    else
    {
        for(int i=0;i<history_bufer.size();i++)
        {
            usleep(100000);
            mvwprintw(window, 13-i, 3,"%d:\t%s",i+1,  history_bufer[i].c_str());
            wrefresh(window);
        }
    }
}

const string Game::current_date_time() 
{
    time_t now = time(0);
    struct tm tstruct;
    char bufer[99];
    tstruct = *localtime(&now);
    strftime(bufer, sizeof(bufer), "%d-%m-%Y\t%X\t", &tstruct);

    return bufer;
}

void Game::to_file(string buffer)
{
    ofstream myfile ("wyniki.txt", ios::app | ios::binary);
    if(myfile.is_open())
    {
        myfile << buffer << endl;
        myfile.close();
    }
  else  mvwprintw(window, 10, 1, "Unable to open file");
}

void Game::from_file()
{
    string buffer;
    ifstream myfile("wyniki.txt");
    if(myfile.is_open())
    {
        while(getline(myfile, buffer))
            history_bufer.push_back(buffer);

        myfile.close();
    }
    else  
        mvwprintw(window, 10, 1, "Unable to open file");
}

int Game::get_free_field()
{   
    srand(time(0));
    int check_fields[9] = {0};
    int rnd_field;

    for(int i=0; i<charBoard.size();i++)
        if(charBoard[i] != '_')
            check_fields[i] = 1;
    
    for(int i=0; i<999;i++) 
    {
        rnd_field= rand()%9;
        if(check_fields[rnd_field] == 0)
            return rnd_field;
    }
}
