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

template <char C1,char C2>
constexpr unsigned char form_hex()
{
	return FormHex<C1, C2>::val;
}

template<char ...CS>
struct string{
	template<std::size_t I>
	constexpr static char get()
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

	constexpr static size_t len()
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

template <typename S>
constexpr decltype(auto) prepare(S s) {
	return prepare_impl(s,
		std::make_index_sequence< sizeof(S::get()) - 1>{});
}

template<typename T>
struct rgba;

template<char ... Cs>
struct rgba<string<Cs...>>
{
	using type = string<Cs...>;
	static_assert(type::len() == 8);
	static constexpr float value[4] = { 
		(float)(form_hex<type::template get<0>(),type::template get<1>()>()) / 255.0f  ,
		(float)(form_hex<type::template get<2>(),type::template get<3>()>()) / 255.0f  ,
		(float)(form_hex<type::template get<4>(),type::template get<5>()>()) / 255.0f  ,
		(float)(form_hex<type::template get<6>(),type::template get<7>()>()) / 255.0f
	};

	template<typename T>
	T make()
	{
		T t;
		t.r = value[0];
		t.g = value[1];
		t.b = value[2];
		t.a = value[3];
		return t;
	}
};



template<char ...Cs>
auto make_rgba(string<Cs...> str) -> rgba<string<Cs...>>
{
	return {};
}

}


#define PREPARE_STRING(s)                                                \
    (::wws::prepare([]{														\
        struct tmp {                                                        \
            static constexpr decltype(auto) get() { return s; }             \
        };                                                                  \
        return tmp{};                                                       \
    }())) 

struct Color {
	float r;
	float g;
	float b;
	float a;
};

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


	Color c = wws::make_rgba(PREPARE_STRING("1200ffff")).make<Color>();
	dbg(std::make_tuple(c.r, c.g, c.b, c.a));

#ifdef WIN32
	system("pause");
#endif // WIN32

	return 0;
}
