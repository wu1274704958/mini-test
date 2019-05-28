#include <dbg.hpp>

namespace wws {


	template<typename T,size_t N>
	struct vec {
	public:
		vec()
		{
			memset(data, 0,bits());
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
		static constexpr size_t bits()
		{
			return sizeof(T) * N;
		}
		template<size_t I>
		void set(int i,T t)
		{
			if constexpr(I <= N)
			{
				if (i == I)
					data[I - 1] = t;
				else
					set<I + 1>(i, t);
			}
		}
	private:
		T data[N];
	};

	typedef vec<float, 3> vec3;
}