#include "mazeboard.h"

#include <random>
#include <chrono>
#include <thread>
#include <exception>

namespace
{
    void remove_wall(Cell::Coords icoords, MazeBoard::Dir idir)
    {
        switch (idir)
        {
        case MazeBoard::Dir::UP:
            if(icoords.y<DIM-1)
                MazeBoard::get()[icoords]->wall_.BOTTOM=false;
            break;
        case MazeBoard::Dir::DOWN:
            if(icoords.y>0)
                MazeBoard::get()[icoords]->wall_.TOP=false;
            break;
        case MazeBoard::Dir::RIGHT:
            if(icoords.x>0)
                MazeBoard::get()[icoords]->wall_.LEFT=false;
            break;
        case MazeBoard::Dir::LEFT:
            if(icoords.x<DIM-1)
                MazeBoard::get()[icoords]->wall_.RIGHT=false;
            break;
        }
    }

    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine eng(seed);

    void change_cells(Cell::Coords icoords = {0,0}, MazeBoard::Dir idir = MazeBoard::Dir::DOWN)
    {
        if(icoords.x<0 || icoords.x>=DIM) return;
        if(icoords.y<0 || icoords.y>=DIM) return;

        if(MazeBoard::get()[icoords]->is_visited) return;
        MazeBoard::get()[icoords]->is_visited = true;

        remove_wall(icoords, idir);

        std::uniform_int_distribution<short> distr(0,4);
        short r = distr(eng);
        for(int n=0; n<4; ++n)
        {
            switch (r)
            {
            case 1:
                change_cells({icoords.x,icoords.y-1},MazeBoard::Dir::UP);
                break;
            case 2:
                change_cells({icoords.x+1,icoords.y},MazeBoard::Dir::RIGHT);
                break;
            case 3:
                change_cells({icoords.x,icoords.y+1},MazeBoard::Dir::DOWN);
                break;
            case 4:
                change_cells({icoords.x-1,icoords.y},MazeBoard::Dir::LEFT);
                break;
            }

            if(++r>4)r=1;

        }
    }

    void connect_walls()
    {
        Cell::Coords c{0,0};
        for(auto &x : MazeBoard::get())
        {
            c = x->get_coords();
            if(!x->wall_.TOP && c.y>0)
            {
                MazeBoard::get()[{c.x,c.y-1}]->wall_.BOTTOM = false;
            }
            if(!x->wall_.BOTTOM && c.y<DIM-1)
            {
                MazeBoard::get()[{c.x,c.y+1}]->wall_.TOP = false;
            }
            if(!x->wall_.LEFT && c.x>0)
            {
                MazeBoard::get()[{c.x-1,c.y}]->wall_.RIGHT = false;
            }
            if(!x->wall_.RIGHT && c.x<DIM-1)
            {
                MazeBoard::get()[{c.x+1,c.y}]->wall_.LEFT = false;
            }
        }
    }
}

MazeBoard *MazeBoard::instance_ = nullptr;

MazeBoard::MazeBoard()
{
    for(auto &c : board_)
    {
        c = std::unique_ptr<Cell>(new Cell( get_coords(&c).x, get_coords(&c).y ));
    }
}

MazeBoard::MazeBoard(const MazeBoard &iclass)
{
    instance_ = iclass.instance_;
}

MazeBoard &MazeBoard::operator=(const MazeBoard &iclass)
{
    if(this != &iclass)
        instance_ = iclass.instance_;
    return *this;
}

MazeBoard &MazeBoard::get()
{
    static MazeBoard temp;
    instance_ = &temp;

    return *instance_;
}

MazeBoard::CellPtr &MazeBoard::operator[](const Cell::Coords &icoords)
{
    return board_[icoords.y*DIM + icoords.x];
}

const MazeBoard::CellPtr &MazeBoard::operator[](const Cell::Coords &icoords) const
{
    return board_[icoords.y*DIM + icoords.x];
}

MazeBoard::Board::iterator MazeBoard::begin()
{
    return board_.begin();
}

MazeBoard::Board::iterator MazeBoard::end()
{
    return board_.end();
}

Cell::Coords MazeBoard::get_coords(const MazeBoard::CellPtr *cell) const
{
    auto dist = std::distance(board_.begin(), cell);
    if( dist>=0 && dist<DIM*DIM )
    {
        int y = static_cast<int>(dist/DIM);
        int x = dist - (DIM*y);
        return Cell::Coords{x,y};
    }
    else
    {
        throw std::invalid_argument("cell not found");
    }
}

void MazeBoard::generate()
{
    change_cells();
    connect_walls();
    save_to_file();
    print_board();

}
