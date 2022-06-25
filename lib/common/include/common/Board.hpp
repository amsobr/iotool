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
    
    /**
     * @brief find all peripherals of a given type
     * @param t The type of peripheral to look for
     * @return A vector with all peripherals of given type. Will be empty of
     *          there is no peripheral that matches.
     */
    [[nodiscard]] std::vector<PeripheralPtr> findPeripherals( PeripheralType t ) const
    {
        std::vector<PeripheralPtr> result;
        for ( auto const& p : myPeripherals ) {
            if ( p->getType()==t ) {
                result.emplace_back(p);
            }
        }
        return result;
    }

    /**
     * @brief Get a peripheral by name
     * @tparam T The type of peripheral. Must match the class of the object or
     *           any of its base classes
     * @param alias The name of the peripheral. Usually it is CWD from the
     *              context
     * @return A shared pointer to the peripheral by the given name
     * @throws runtime_error in case the peripheral
     * @throws may throw if given T is incompatible with the class of the
     *          peripheral
     */
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

