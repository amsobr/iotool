#pragma once


#include <string>
#include <vector>
#include <memory>

#include <common/PeripheralType.hpp>
#include <common/Peripheral.hpp>


class Board;
typedef std::shared_ptr<Board> BoardPtr;

class Board
{
public:
    static BoardPtr get()
    {
        return myInstance;
    }

    /** @brief create the board instance.
     *
     * <em>This method is not thread-safe, and @em MUST be called before
     * get() </em>
     */
    static void create();

    Board() = default;
    virtual ~Board() = default;

    [[nodiscard]] std::vector<PeripheralPtr> const& getPeripherals() const
    {
        return myPeripherals;
    }

    template< typename T>
    [[nodiscard]] std::shared_ptr<T> getPeripheral(std::string const& alias ) const
    {
        for ( auto const& p : myPeripherals ) {
            if ( p->getAlias()==alias ) {
                return std::dynamic_pointer_cast<T>(p);
            }
        }
        throw std::runtime_error{ "peripheral not found: " + alias };
    }

protected:
    static BoardPtr               myInstance;

    std::vector<PeripheralPtr>    myPeripherals;

}; /* class Board */

