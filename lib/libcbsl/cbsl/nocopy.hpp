#if !defined( NO_COPY_HPP )
#define NO_COPY_HPP

/** \brief Explicitly disable copy constructor and assigment operator */
#define WC_DISABLE_COPY(class) \
class( class const &other ); \
class &operator=( class const &other ); \

#define CBSL_NO_COPY(class) \
class( class const &other ); \
class &operator=( class const &other ); \

#endif /* !defined(NO_COPY_HPP) */
