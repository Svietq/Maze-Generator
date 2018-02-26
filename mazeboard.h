#ifndef MAZEBOARD_H
#define MAZEBOARD_H

#include "cell.h"

#include <vector>
#include <iterator>
#include <memory>
#include "windows.h"
#include <iostream>
#include <fstream>

constexpr short DIM = 16;
constexpr short WIDTH = 1;

class MazeBoard
{
public:
    using CellPtr = std::unique_ptr<Cell>;

    using Board = std::array<CellPtr, DIM*DIM>;

    static MazeBoard &get();
    CellPtr& operator[](const Cell::Coords &icoords);
    const CellPtr& operator[](const Cell::Coords &icoords) const;
    Board::iterator begin();
    Board::iterator end();
    Cell::Coords get_coords(const CellPtr* cell) const;

    enum class Dir {UP, DOWN, LEFT, RIGHT};
    void generate();

private:
    MazeBoard();
    MazeBoard(const MazeBoard &iclass);
    MazeBoard &operator=(const MazeBoard &iclass);
    ~MazeBoard(){}

    static MazeBoard *instance_;
    Board board_;

};

namespace
{
    void gotoxy(short ix, short iy)
    {
        static HANDLE h = NULL;
          if(!h)
            h = GetStdHandle(STD_OUTPUT_HANDLE);
          COORD c = { ix, iy };
          SetConsoleCursorPosition(h,c);
    }

    void gotoxy(std::ofstream &file, int ix, int iy)
    {
        file.seekp(iy*((DIM*2+2)*WIDTH+1)+ix);  //WTF?!
    }

    void print(const char &c)
    {
        using std::cout;
        cout.width(WIDTH);
        cout << c;
    }

    void print_board()
    {
        for(const MazeBoard::CellPtr &c : MazeBoard::get())
        {
            int x = MazeBoard::get().get_coords(&c).x * WIDTH * 2; //*2 - . . . . .     *3 - . . . . . .
            int y = MazeBoard::get().get_coords(&c).y * 2;         //     .   .   .          .   . .   .
                                                                   //     . . . . .          . . . . . .
            gotoxy(x,y);

            print('.');
            if(c->wall_.TOP)    print('.');     //. . .
            else print(' ');
            print('.');

            gotoxy(x,y+1);

            if(c->wall_.LEFT)   print('.');
            else print(' ');
            print(' ');                         //.   .
            if(c->wall_.RIGHT)  print('.');
            else print(' ');
            gotoxy(x,y+2);

            print('.');
            if(c->wall_.BOTTOM) print('.');     //. . .
            else print(' ');
            print('.');
        }
    }

    void print(std::ofstream &file, const char &c)
    {
        file.width(WIDTH);
        file << c;
    }

    void write_grid(std::ofstream &file)
    {
        for(int i=0; i<(DIM*2+1); ++i)
        {
            for(int j=0; j<(DIM*2+1); ++j)
            {
                print(file, '*');
            }
            print(file, '\n');
        }
    }

    void save_to_file()
    {
        std::ofstream file;
        file.open("maze.txt", std::ofstream::out | std::ofstream::trunc);
        write_grid(file);

        for(const MazeBoard::CellPtr &c : MazeBoard::get())
        {
            int x = MazeBoard::get().get_coords(&c).x * WIDTH * 2; //*2 - . . . . .     *3 - . . . . . .
            int y = MazeBoard::get().get_coords(&c).y * 2;         //     .   .   .          .   . .   .
                                                                   //     . . . . .          . . . . . .
            gotoxy(file,x,y);

            print(file, '.');
            if(c->wall_.TOP)    print(file, '.');     //. . .
            else print(file, ' ');
            print(file, '.');

            gotoxy(file,x,y+1);

            if(c->wall_.LEFT)   print(file, '.');
            else print(file, ' ');
            print(file, ' ');                         //.   .
            if(c->wall_.RIGHT)  print(file, '.');
            else print(file, ' ');
            gotoxy(file,x,y+2);

            print(file, '.');
            if(c->wall_.BOTTOM) print(file, '.');     //. . .
            else print(file, ' ');
            print(file, '.');

        }

        file.close();
    }
}

#endif // MAZEBOARD_H
