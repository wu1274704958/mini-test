#include <iostream>
#include "dbg.hpp"

namespace wws{

template<char C1,char C2>
	struct FormHex{
	template<char C>
	constexpr static unsigned char get()
	{
		if constexpr(C >= '0' && C <= '9')
		{
			return C - '0';
		}else if constexpr (C >= 'A' && C <= 'F' ){
			return 10 + (C - 'A');
		}else if constexpr (C >= 'a' && C <= 'f' ){
			return 10 + (C - 'a');
		}
		return 0;
	}
	constexpr static unsigned char val = get<C1>() * 16 + get<C2>();
};

template <typename T>
struct rgba
{
	constexpr static char val = 0;
};

template<char ...CS>
struct string {
	template<std::size_t I>
	constexpr char get()
	{
		if constexpr(I < 0 || I > len_impl<1, CS...>())
			return 0;
		else
			return get_impl<0,I,CS...>();
	}

	template<std::size_t B,std::size_t I,char Curr,char ...Cs>
	constexpr static char get_impl()
	{
		if constexpr (B == I)
		{
			return Curr;
		}
		else {
			return get_impl<B + 1, I, Cs...>();
		}
	}

	template<std::size_t B, std::size_t I>
	constexpr static char get_impl()
	{
		return 0;
	}

	constexpr size_t len()
	{
		return len_impl<1, CS...>();
	}

	template<std::size_t B, char Curr, char ...Cs>
	constexpr static char len_impl()
	{
		if constexpr (sizeof...(Cs) == 0)
		{
			return B;
		}
		else {
			return len_impl<B + 1, Cs...>();
		}
	}
};

template <typename S, std::size_t ...N>
constexpr string<S::get()[N]...>
 prepare_impl(S, std::index_sequence<N...>)
{
	return {};
}

template <typename T>
struct AL;

template <typename T, size_t L>
struct AL<T[L]>
{
	using type = T;
	constexpr static size_t val = L;
};

template <typename S>
constexpr decltype(auto) prepare(S s) {
	return prepare_impl(s,
		std::make_index_sequence< sizeof(S::get()) - 1>{});
}

}


#define PREPARE_STRING(s)                                                \
    (::wws::prepare([]{														\
        struct tmp {                                                        \
            static constexpr decltype(auto) get() { return s; }             \
        };                                                                  \
        return tmp{};                                                       \
    }())) 

int main()
{
	dbg((int)wws::FormHex<'A','f'>::val);
	dbg(0xaf);
	
	auto str = PREPARE_STRING("aa");
	dbg(str.get<0>());
	dbg(str.get<1>());
	dbg(str.get<2>());
	dbg(str.get<100>());
	dbg(str.len());

#ifdef WIN32
	system("pause");
#endif // WIN32

	return 0;
}
