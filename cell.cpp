#include "cell.h"
#include <iostream>

Cell::Cell(int ix, int iy) : coords_({ix,iy}) {}
Cell::Cell(Cell::Coords icoords) : Cell(icoords.x, icoords.y) {}

const Cell::Coords &Cell::get_coords() const
{
    return coords_;
}
