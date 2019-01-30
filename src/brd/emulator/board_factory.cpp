#include <common/board.hpp>

#include "emulator_board.hpp"


BoardPtr createBoard()
{
    return BoardPtr( new EmulatorBoard() );
}