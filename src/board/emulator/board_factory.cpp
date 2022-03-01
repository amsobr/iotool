
#include <common/Board.hpp>

#include "EmulatorBoard.hpp"


BoardPtr createBoard()
{
    return BoardPtr( new EmulatorBoard() );
}
