#pragma once
#include <dbg.hpp>
#include <const_val.hpp>
#include <cmath>
#include <cassert>

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

    template <typename T,size_t M,size_t N>
    struct matrix;

    template <typename T,size_t M,size_t N>
    std::ostream &operator<<(std::ostream& out,matrix<T,M,N>& m)
    {
        out << '\n';
        for (int i = 0; i < M; ++i) {
            out << m.data[i];
            if(i != M - 1)
                out << '\n';
        }
        return out;
    }

	template<typename T,size_t M,size_t N>
	matrix<T, M, N> operator*(T t, matrix<T, M, N> m)
	{
		matrix<T, M, N> res;
		for (size_t i = 1; i <= M; ++i)
		{
			for (size_t j = 1; j <= N; ++j)
			{
				res.set(i, j, m.data[i - 1][j - 1] * t);
			}
		}
		return res;
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
				set(i++, *it);
			}
		}
		
		T& get(size_t i)
		{
			assert((i > 0 && i <= N));
			return data[i - 1];
		}
		template<size_t I>
		T& get_()
		{
            static_assert((I > 0 && I <= N),"Out of bound!");
			return data[I - 1];
		}
		T& x()
		{
			return get_<1>();
		}
		T& y()
		{
			return get_<2>();
		}
		T& z()
		{
			return get_<3>();
		}
		T& w()
		{
			return get_<4>();
		}
        T& r()
        {
            return get_<1>();
        }
        T& g()
        {
            return get_<2>();
        }
        T& b()
        {
            return get_<3>();
        }
        T& a()
        {
            return get_<4>();
        }
		static constexpr size_t byte_size()
		{
			return sizeof(T) * N;
		}
		void set(size_t i, T t)
		{
			assert(i > 0 && i <= N);
			data[i - 1] = t;
		}
		template<size_t I>
		void set_(size_t i,T t)
		{
            static_assert((I > 0 && I <= N),"Out of bound!");
			if constexpr(I > 0 && I < N)
			{
				if (i == I)
					data[I - 1] = t;
				else
					set_<I + 1>(i, t);
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
        T& operator[](size_t i)
        {
            return data[i];
        }
        friend std::ostream& operator<<<T,N>(std::ostream& out,vec<T,N>& v);
	private:
		T data[N];
	};

    template <typename T,size_t M,size_t N>
    struct matrix{
	private:
        vec<T,N> data[M];
	public:
        friend std::ostream&operator<<<T,M,N>(std::ostream&,matrix<T,M,N>&);
		friend matrix<T, M, N> operator*<T, M, N>(T, matrix<T, M, N>);
        static constexpr size_t byte_size()
        {
            return sizeof(vec<T,N>) * M;
        }
        matrix()
        {
            memset(data,0,byte_size());
        }
        matrix(T t)
        {
            size_t j = 1;
            for (int i = 0; i < M; ++i) {
                data[i].set(j++,t);
            }
        }
        matrix(std::initializer_list<T> list)
        {
            size_t i = 1,j = 1;
            for(auto it = list.begin();it != list.end();++it)
            {
                set(i,j,*it);
                if(j == N)
                {
                    ++i;
                    j = 1;
                }else{
                    ++j;
                }
            }
        }
		vec<T, N> row(int i)
		{
			assert(i > 0 && i <= M);
			return data[i - 1];
		}
		T& get(size_t i,size_t j)
		{
			assert((i > 0 && i <= M) && (j > 0 && j <= N));
			return data[i - 1].get(j);
		}
        template<size_t I,size_t J>
        T& get_()
        {
            static_assert((I > 0 && I <= M ) && (J > 0 && J <= N),"Out of bound!");
            return data[I - 1].get_<J>();
        }
		void set(size_t i, size_t j, T t)
		{
			assert((i > 0 && i <= M) && (j > 0 && j <= N));
			data[i - 1][j - 1] = t;
		}
        template<size_t I,size_t J>
        void set_(size_t i,size_t j,T t)
        {
            static_assert((I > 0 && I <= M ) && (J > 0 && J <= N),"Out of bound!");
            if constexpr(J > 0 && J < N)
            {
                if(i == I && j == J)
                    data[I - 1].set(j,t);
                else
                    set_<I,J + 1>(i,j,t);
            }else if constexpr (I == M)
            {
                if(i == I && j == J)
                    data[I - 1].set(j,t);
            }else if constexpr (J == N)
            {
                if(i == I && j == J)
                    data[I - 1].set(j,t);
                else
                    set_<I + 1,1>(i,j,t);
            }
        }

        T& m11(){ return get_<1,1>(); }
        T& m12(){ return get_<1,2>(); }
        T& m13(){ return get_<1,3>(); }
        T& m14(){ return get_<1,4>(); }
							
        T& m21(){ return get_<2,1>(); }
        T& m22(){ return get_<2,2>(); }
        T& m23(){ return get_<2,3>(); }
        T& m24(){ return get_<2,4>(); }
							
        T& m31(){ return get_<3,1>(); }
        T& m32(){ return get_<3,2>(); }
        T& m33(){ return get_<3,3>(); }
        T& m34(){ return get_<3,4>(); }
							
        T& m41(){ return get_<4,1>(); }
        T& m42(){ return get_<4,2>(); }
        T& m43(){ return get_<4,3>(); }
        T& m44(){ return get_<4,4>(); }

        matrix<T,N,M> transpose()
        {
            matrix<T,N,M> mat;
            size_t i = 1,j = 1;
            for(;;)
            {
                mat.set(j,i,data[i - 1][j - 1]);
                if(j == N)
                {
                    if(i == M)break;
                    ++i;j = 1;
                }else{
                    ++j;
                }
            }
            return mat;
        }
		vec<T, M> operator*(vec<T, N> v)
		{
			vec<T, M> res;
			for (size_t i = 1; i <= M; ++i)
			{
				res.set(i, data[i - 1] * v);
			}
			return res;
		}
		template<size_t ON>
		matrix<T, M, ON> operator*(matrix<T, N, ON> m)
		{
			matrix<T, ON, N> tped = m.transpose();
			matrix<T, M, ON> res;
			for (size_t i = 1; i <= M; ++i)
			{
				for (size_t j = 1; j <= ON; ++j)
				{
					res.set(i, j, data[i - 1] * tped.row(j));
				}
			}
			return res;
		}
    };

    typedef matrix<float,2,2> mat2;
    typedef matrix<float,3,3> mat3;
    typedef matrix<float,4,4> mat4;
	typedef matrix<float, 3, 2> mat3x2;
	typedef matrix<float, 2, 3> mat2x3;
	typedef matrix<float, 3, 4> mat3x4;
	typedef matrix<float, 4, 3> mat4x3;

	typedef vec<float, 2> vec2;
	typedef vec<float, 3> vec3;
	typedef vec<float, 4> vec4;
}