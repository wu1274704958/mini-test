//
// Created by 吴帅杰 on 2019/5/29.
//

#ifndef HELLO_CONST_VAL_H
#define HELLO_CONST_VAL_H

#include <type_traits>

namespace const_val{
    template <typename T,typename = std::enable_if_t<(std::is_integral_v<T> || std::is_floating_point_v<T>) >>
    constexpr T zero()
    {
        return static_cast<T>(0);
    }

    template <typename T,typename = std::enable_if_t<(std::is_integral_v<T> || std::is_floating_point_v<T>) >>
    constexpr T one()
    {
        return static_cast<T>(1);
    }

    template <typename T,typename = std::enable_if_t< std::is_floating_point_v<T> >>
    constexpr T pi()
    {
        return static_cast<T>(3.1415926535997932384626433832795);
    }

    template <typename T,typename = std::enable_if_t<std::is_floating_point_v<T>> >
    constexpr T pi_2()
    {
        return pi<T>() * static_cast<T>(2);
    }

    template <typename T,typename = std::enable_if_t<std::is_floating_point_v<T>> >
    constexpr T pi_1_2()
    {
        return pi<T>() * static_cast<T>(0.5);
    }
}

#endif //HELLO_CONST_VAL_H
