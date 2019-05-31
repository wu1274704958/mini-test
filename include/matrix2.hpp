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
		template<size_t I>
		T& get()
		{
            static_assert((I > 0 && I <= N),"Out of bound!");
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
		template<size_t I>
		void set(size_t i,T t)
		{
            static_assert((I > 0 && I <= N),"Out of bound!");
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
        vec<T,N> data[M];
        friend std::ostream&operator<<<T,M,N>(std::ostream&,matrix<T,M,N>&);
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
                data[i].set<1>(j++,t);
            }
        }
        matrix(std::initializer_list<T> list)
        {
            size_t i = 1,j = 1;
            for(auto it = list.begin();it != list.end();++it)
            {
                set<1,1>(i,j,*it);
                if(j == N)
                {
                    ++i;
                    j = 1;
                }else{
                    ++j;
                }
            }
        }
        template<size_t I,size_t J>
        T& get()
        {
            static_assert((I > 0 && I <= M ) && (J > 0 && J <= N),"Out of bound!");
            return data[I - 1].get<J>();
        }
        template<size_t I,size_t J>
        void set(size_t i,size_t j,T t)
        {
            static_assert((I > 0 && I <= M ) && (J > 0 && J <= N),"Out of bound!");
            if constexpr(J > 0 && J < N)
            {
                if(i == I && j == J)
                    data[I - 1].set<J>(j,t);
                else
                    set<I,J + 1>(i,j,t);
            }else if constexpr (I == M)
            {
                if(i == I && j == J)
                    data[I - 1].set<J>(j,t);
            }else if constexpr (J == N)
            {
                if(i == I && j == J)
                    data[I - 1].set<J>(j,t);
                else
                    set<I + 1,1>(i,j,t);
            }
        }

        T& m11(){ return get<1,1>(); }
        T& m12(){ return get<1,2>(); }
        T& m13(){ return get<1,3>(); }
        T& m14(){ return get<1,4>(); }

        T& m21(){ return get<2,1>(); }
        T& m22(){ return get<2,2>(); }
        T& m23(){ return get<2,3>(); }
        T& m24(){ return get<2,4>(); }

        T& m31(){ return get<3,1>(); }
        T& m32(){ return get<3,2>(); }
        T& m33(){ return get<3,3>(); }
        T& m34(){ return get<3,4>(); }

        T& m41(){ return get<4,1>(); }
        T& m42(){ return get<4,2>(); }
        T& m43(){ return get<4,3>(); }
        T& m44(){ return get<4,4>(); }

        matrix<T,N,M> transpose()
        {
            matrix<T,N,M> mat;
            size_t i = 1,j = 1;
            for(;;)
            {
                mat.set<1,1>(j,i,data[i - 1][j - 1]);
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
    };

    typedef matrix<float,2,2> mat2;
    typedef matrix<float,3,3> mat3;
    typedef matrix<float,4,4> mat4;

	typedef vec<float, 2> vec2;
	typedef vec<float, 3> vec3;
	typedef vec<float, 4> vec4;
}