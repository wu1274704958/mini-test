#pragma once
#include <dbg.hpp>
#include <const_val.hpp>
#include <cmath>

namespace cgm {


    template<typename T,size_t N>
    struct vec;

    template <typename T,size_t N>
    std::ostream &operator<<(std::ostream& out,vec<T,N>& v)
    {
        out << '[';
        for (int i = 0; i < N; ++i) {
            out << v.data[i];
            if(i != N - 1)
                out << ',';
        }
        out << ']';
        return out;
    }

	template<typename T,size_t N>
	struct vec {
        static_assert(std::is_floating_point_v<T>,"T must be floating point !");
	public:
		vec()
		{
			memset(data, 0,byte_size());
		}
		vec(T t)
		{
			for (int i = 0; i < N; ++i)
				data[i] = t;
		}
		vec(std::initializer_list<T> list)
		{
			int i = 1;
			for (auto it = list.begin(); it != list.end(); ++it)
			{
				set<1>(i++, *it);
			}
		}
		template<size_t I,typename = std::enable_if_t<(I > 0 && I <= N)> >
		T& get()
		{
			return data[I - 1];
		}
		T& x()
		{
			return get<1>();
		}
		T& y()
		{
			return get<2>();
		}
		T& z()
		{
			return get<3>();
		}
		T& w()
		{
			return get<4>();
		}
        T& r()
        {
            return get<1>();
        }
        T& g()
        {
            return get<2>();
        }
        T& b()
        {
            return get<3>();
        }
        T& a()
        {
            return get<4>();
        }
		static constexpr size_t byte_size()
		{
			return sizeof(T) * N;
		}
		template<size_t I,typename = std::enable_if_t<(I > 0 && I <= N)> >
		void set(int i,T t)
		{
			if constexpr(I > 0 && I < N)
			{
				if (i == I)
					data[I - 1] = t;
				else
					set<I + 1>(i, t);
			}else if constexpr (I == N){
                if (i == I)
                    data[I - 1] = t;
			}
		}

		vec<T,N> operator+(vec<T,N> v)
        {
		    vec<T,N> res;
		    for(int i = 0;i < N;++i)
            {
		        res.data[i] = data[i] + v.data[i];
            }
            return res;
        }
        vec<T,N> operator-(vec<T,N> v)
        {
            vec<T,N> res;
            for(int i = 0;i < N;++i)
            {
                res.data[i] = data[i] - v.data[i];
            }
            return res;
        }
        vec<T,N> operator-()
        {
            vec<T,N> res;
            for(int i = 0;i < N;++i)
            {
                res.data[i] = const_val::zero<T>() - data[i];
            }
            return res;
        }
        T len()
        {
            T res = const_val::zero<T>();
		    for(int i = 0;i < N;++i)
            {
		        res += std::pow(data[i], static_cast<T>(2));
            }
		    return std::sqrt(res);
        }
        vec<T,N> unitized()
        {
		    vec<T,N> res;
		    T l = len();
            T ratio = const_val::one<T>() / l;
            for(int i = 0;i < N;++i)
            {
                res.data[i] = data[i] * ratio;
            }
            return res;
        }
        T operator*(vec<T,N> v)
        {
		    T res = const_val::zero<T>();
            for(int i = 0;i < N;++i)
            {
                res += data[i] * v.data[i];
            }
            return res;
        }
        vec<T,N> operator*(T t)
        {
		    vec<T,N> res;
            for (int i = 0; i < N; ++i) {
                res.data[i] = data[i] * t;
            }
            return res;
        }
        friend std::ostream& operator<<<T,N>(std::ostream& out,vec<T,N>& v);
	private:
		T data[N];
	};

    template <typename T,size_t M,size_t N>
    struct matrix{
        vec<T,M> data[N];
        static constexpr size_t byte_size()
        {
            return sizeof(vec<T,M>) * N;
        }
        matrix()
        {
            memset(data,0,byte_size());
        }
        matrix(T t)
        {
            for (int i = 0; i < N; ++i) {
                data[i] = vec<T,M>(t);
            }
        }
        matrix(std::initializer_list<T> list)
        {

        }
//        template<size_t I,typename = std::enable_if_t<(I > 0 && I <= M * N)> >
//        T& get()
//        {
//            return data[];
//        }
    };

	typedef vec<float, 2> vec2;
	typedef vec<float, 3> vec3;
	typedef vec<float, 4> vec4;
}