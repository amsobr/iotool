#ifndef EXCEPTION_H
#define EXCEPTION_H

namespace Pti {

class Exception {
	private:
		int	m_code;
		char const *m_message;
                
	public:
		Exception( int newCode , char const *msg=0 ) :
		m_code(newCode) , m_message(msg)
		{
		}
		
        Exception( Exception const &other ) :
        m_code(other.m_code) ,
        m_message(other.m_message)
        {
        }
		
		Exception const &operator=( Exception const &other )
        {
            m_code      = other.m_code;
            m_message   = other.m_message;
            return *this;
        }
		
		virtual ~Exception() {};
		
		int getCode() const { return m_code; };
		char const * getMessage() const { return m_message; };
}; /* end class exception */
	
}; /* end namespace Pti */


#endif /* EXCEPTION_H */
