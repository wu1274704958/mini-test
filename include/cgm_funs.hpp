#pragma once

#include <matrix2.hpp>
#include <const_val.hpp>
#include <cmath>
#include <type_traits>

namespace cgm{

    template<typename T,typename = std::enable_if_t< std::is_floating_point_v<T> >>
    T radians(T t)
    {
        return t / static_cast<T>(180) * const_val::pi<T>();
    }

    template<typename T,typename = std::enable_if_t< std::is_floating_point_v<T> >>
    T degrees(T t)
    {
        return t / const_val::pi<T>() * static_cast<T>(180);
    }

    template <typename T,typename = std::enable_if_t< std::is_floating_point_v<T> >>
    T angle(vec<T,2> v1,vec<T,2> v2)
    {
        return degrees(std::acos(v1.unitized() * v2.unitized()));
    }

    template <typename T,typename = std::enable_if_t< std::is_floating_point_v<T> >>
    vec<T,3> cross(vec<T,3> v1,vec<T,3> v2)
    {
        return vec<T,3>{  v1.y() * v2.z() - v1.z() * v2.y() ,
                           v1.z() * v2.x() - v1.x() * v2.z() ,
                           v1.x() * v2.y() - v1.y() * v2.x()
        };
    }
}
