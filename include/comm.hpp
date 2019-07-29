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


	constexpr bool big_endian()
	{
		union Temp
		{
			int _1;
			char _2[4];
		};

		constexpr Temp temp = {1};
		if (temp._2[0] == static_cast<char>(1))
			return true;
		else
			return false;
	}
}