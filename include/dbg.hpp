#pragma once

#include <iostream>
#include <utility>

namespace wws{
	template <bool B, typename T>
	struct has_type;

	template <typename T>
	struct has_type<true, T> {
		using type = T;
		static constexpr bool val = true;
	};

	template <>
	struct has_type<false, void> {
		using type = void;
		static constexpr bool val = false;
	};

	template <bool B, typename T1, typename T2>
	struct choose_if {
		using type = T2;
	};

	template <typename T1, typename T2>
	struct choose_if<true, T1, T2>
	{
		using type = T1;
	};


	template<typename T>
	struct can_cout
	{
		template<typename U>
		static auto func(int) -> decltype((std::declval<std::ostream&>() << std::declval<U>()), std::true_type())
		{
			return std::true_type();
		}
		template<typename U>
		static auto func(...)->std::false_type
		{
			return std::false_type();
		}
		static constexpr bool val = decltype(func<T>(0))::value;
	};

	struct V{

	};

	template<typename T>
	auto dbg_func(const char *expr, T&& t) -> T
	{
		if constexpr (can_cout<T>::val)
		{
			std::cout << expr << " = " << t << std::endl;
		}
		else {
			std::cout << expr << " = " << "This type can not print!!!" << std::endl;
		}
		return std::forward<T>(t);
	}

    template<typename T = V>
    auto dbg_func(const char *expr, V&& t) -> void
    {
        std::cout << expr << " = " << "void" << std::endl;
        return;
    }
}

#define dbg(expr,...)  wws::dbg_func(#expr,(expr,##__VA_ARGS__))
#define dbgv(expr)  dbg(expr,wws::V())