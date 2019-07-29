#pragma once
#include <iostream>

namespace wws {

	struct nonesuch {
		nonesuch() = delete;
		~nonesuch() = delete;
		nonesuch(const nonesuch&) = delete;
		void operator=(const nonesuch&) = delete;
	};

	template<class Default, class AlwaysVoid,
		template<class...> class Op, class... Args>
	struct detector {
		using value_t = std::false_type;
		using type = Default;
	};


	template<class Default, template<class...> class Op, class... Args>
	struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
		using value_t = std::true_type;
		using type = Op<Args...>;
	};

	template<template<class...> class Op, class... Args>
	using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

	template<template<class...> class Op, class... Args>
	using detected_t = typename detector<nonesuch, void, Op, Args...>::type;


    inline bool big_endian()
	{
		union Temp
		{
			short _1;
			char _2[2];
		} temp;

		temp._2[0] = 0x11;
		temp._2[1] = 0x22;
		
		if(temp._1 == static_cast<short>(0x1122))
			return true;
		else
			return false;
	}

	template <typename T,typename = std::enable_if_t<std::is_integral_v<T>>>
	T reverse_byte(T t)
	{
		int len = static_cast<int>(sizeof(T));
		char *p = reinterpret_cast<char*>(&t);
		int e = len - 1;
		int b = 0;
		while ( b < e )
		{
			char temp = p[b];
			p[b] = p[e];
			p[e] = temp;

			++b;
			--e;
		}
		return t;
	}
}
