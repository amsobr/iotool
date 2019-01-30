#ifndef EmulatorBoard_HPP
#define EmulatorBoard_HPP

#include <vector>

#include <common/board.hpp>

class EmulatorBoard : public Board
{
private:

    std::vector<PeripheralPtr> myPeripherals;


public:
    EmulatorBoard();
    virtual ~EmulatorBoard();


    virtual std::vector<PeripheralPtr> getPeripherals();
}; /* class EmulatorBoard */



#endif /* !defined EmulatorBoard_HPP */
