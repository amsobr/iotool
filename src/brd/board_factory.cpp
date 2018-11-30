#include <common/board.hpp>

#include "agp01/agripino.hpp"


BoardPtr createBoard()
{
    return BoardPtr( new Agripino() );
}