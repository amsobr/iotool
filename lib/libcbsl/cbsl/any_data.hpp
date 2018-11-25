/* 
 * File:   any_data.hpp
 * Author: to
 *
 * Created on April 4, 2016, 4:43 PM
 * Credit goes to:
 * http://stackoverflow.com/questions/5450159/type-erasure-techniques
 */

#ifndef ANY_DATA_HPP
#define	ANY_DATA_HPP

namespace cbsl {

class AnyData{
    struct holder_base{
        virtual ~holder_base(){}
        virtual holder_base* clone() const = 0;
    };

    template<class T>
    struct holder : holder_base{
        holder()
        : held_()
        {}

        holder(T const& t)
        : held_(t)
        {}

        virtual ~holder(){
        }

        virtual holder_base* clone() const {
            return new holder<T>(*this);
        }

        T held_;
    };

public:
    AnyData()
    : storage_(0)
    {}

    AnyData(AnyData const& other)
    : storage_(other.storage_->clone())
    {}

    template<class T>
    AnyData(T const& t)
    : storage_(new holder<T>(t))
    {}

    ~AnyData(){
        clear();
    }

    AnyData& operator=(AnyData const& other){
        clear();
        storage_ = other.storage_->clone();
        return *this;
    }

    template<class T>
    AnyData& operator=(T const& t){
        clear();
        storage_ = new holder<T>(t);
        return *this;
    }

    void clear(){
        if(storage_)
            delete storage_;
    }

    template<class T>
    T& as(){
        return static_cast<holder<T>*>(storage_)->held_;
    }

private:
    holder_base* storage_;
};


}; /* namespace cbsl */




#endif	/* ANY_DATA_HPP */

