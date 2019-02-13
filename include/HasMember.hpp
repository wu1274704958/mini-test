#ifndef __WWS_HAS_MEMBER_H__
#define __WWS_HAS_MEMBER_H__
#include <iostream>


#define HAS_MEMBER(n)                                                                                       \
namespace wws{                                                                                              \
    template<typename T>                                                                                    \
    struct Has##n{                                                                                          \
        template<typename U>                                                                                \
        static constexpr auto func(int) -> decltype( std::declval<U>().n,std::true_type() ) {}              \
        template<typename U>                                                                                \
        static constexpr auto func(...) -> std::false_type{ return std::false_type(); }                                               \
        enum {value = decltype(func<T>(0))::value };                                             \
    };                                                                                                      \
}                                                                                                           \


#endif //__WWS_HAS_MEMBER_H__
