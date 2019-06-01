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
	template<typename T,size_t N>
	matrix<T, N - 1, N - 1> cofactor(matrix<T,N,N>& m,size_t a, size_t b)
	{
		matrix<T, N - 1, N - 1> res;
		for (size_t i = 1; i <= N - 1; ++i)
		{
			int fi = i;
			if (i >= a)
				fi += 1;
			for (size_t j = 1; j <= N - 1; ++j)
			{
				int fj = j;
				if (j >= b)
					fj += 1;
				res.set(i, j, m.get(fi, fj));
			}
		}
		return res;
	}

	template<typename T,size_t N>
	T alge_cofactor(matrix<T, N, N>& m, size_t a, size_t b)
	{
		assert((a > 0 && a <= N) && (b > 0 && b <= N));
		T al = ((a + b) % 2 == 0) ? const_val::one<T>() : const_val::zero<T>() - const_val::one<T>();
		auto cof = cofactor(m, a, b);
		return al * det(cof);
	}

	template<typename T,size_t N >
	T det(matrix<T, N, N>& m)
	{
		T res = const_val::zero<T>();
		for (int j = 1; j <= N; ++j)
		{
			res += m.get(1, j) * alge_cofactor(m, 1, j);
		}
		return res;
	}

	template<typename T>
	T det(matrix<T, 2, 2>& m)
	{
		return m.m11()* m.m22() - m.m12() * m.m21();
	}
}
