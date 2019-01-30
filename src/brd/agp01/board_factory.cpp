#include <common/board.hpp>

#include "agripino.hpp"


BoardPtr createBoard()
{
    return BoardPtr( new Agripino() );
}