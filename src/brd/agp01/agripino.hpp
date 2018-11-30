#ifndef AGRIPINO_HPP
#define AGRIPINO_HPP

#include <vector>

#include <common/board.hpp>

class Agripino : public Board
{
private:

    std::vector<PeripheralPtr> myPeripherals;


public:
    Agripino();
    virtual ~Agripino();


    virtual std::vector<PeripheralPtr> getPeripherals();
}; /* class Agripino */



#endif /* !defined AGRIPINO_HPP */
