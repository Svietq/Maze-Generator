#ifndef CELL_H
#define CELL_H

class Cell
{
public:
    struct Coords { int x; int y;};

    Cell(int ix, int iy);
    Cell(Coords icoords);
    Cell(const Cell&) = default;
    Cell(Cell&&) noexcept = default;
    Cell& operator=(const Cell&) = default;
    Cell& operator=(Cell&&)= default;

    bool is_visited = false;
    struct Wall
    {
        bool TOP;
        bool BOTTOM;
        bool LEFT;
        bool RIGHT;
    }wall_{true,true,true,true};
    const Coords &get_coords() const;

private:
    Coords coords_{0,0};
};

#endif // CELL_H
