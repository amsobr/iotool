#ifndef BOARD_DESCRIPTOR_HPP
#define BOARD_DESCRIPTOR_HPP

#include <string>
#include <vector>
#include <memory>

#include "peripheral_type.hpp"
#include "peripheral.hpp"


class Board
{
private:
    std::string myName;
    std::string myRev;

    Board();

protected:
    Board( std::string name , std::string rev ) :
    myName(name) ,
    myRev(rev)
    {
    }

public:
    virtual ~Board() {}

    std::string getName() const { return myName; }
    std::string getRevision() const { return myRev; }

    virtual std::vector<PeripheralPtr> getPeripherals() = 0;
}; /* class Board */

typedef std::shared_ptr<Board> BoardPtr;


#endif /* !defined BOARD_DESCRIPTOR_HPP */
