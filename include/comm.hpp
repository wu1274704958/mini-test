#pragma once
#include <iostream>

#if defined(__clang__) || defined(__GNUC__)
#define CPP_STANDARD __cplusplus
#elif defined(_MSC_VER)
#define CPP_STANDARD _MSVC_LANG
#endif

#if CPP_STANDARD > 199711L
#if CPP_STANDARD > 201103L
#if CPP_STANDARD > 201402L
#if CPP_STANDARD > 201703L
#define __CPP_STD_VER 20
#else
#define __CPP_STD_VER 17
#endif
#else
#define __CPP_STD_VER 14
#endif
#else
#define __CPP_STD_VER 11
#endif
#else
#define __CPP_STD_VER 03
#endif

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

	template<typename T>
	struct arr_len;

	template<typename T, size_t N>
	struct arr_len<T[N]>
	{
		const static size_t val = N;
	};

	template<typename T, size_t N>
	struct arr_len<T(&)[N]>
	{
		const static size_t val = N;
	};

	template<typename T>
	size_t arr_len_v = arr_len<T>::val;

	template<typename Ret, typename T>
	constexpr Ret arrLen(T&& t)
	{
		return static_cast<Ret>(arr_len_v<T>);
	}

	template<typename T>
	constexpr size_t arrLen(T&& t)
	{
		return arr_len_v<T>;
	}
}
#if __CPP_STD_VER >= 20 
// compile time string 
namespace wws {

	template <typename S, std::size_t ...N>
	constexpr std::array<std::remove_reference_t<decltype(S::get()[0])>, sizeof...(N)>
		prepare_std_arr_impl(S, std::index_sequence<N...>)
	{
		return { { S::get()[N] ... } };
	}


	template <typename S>
	constexpr decltype(auto) prepare_std_arr(S s) {
		return prepare_std_arr_impl(s,
			std::make_index_sequence< sizeof(S::get())>{});
	}

	template<typename T,size_t N>
	struct ct_string {
		static_assert(std::is_same_v<T,const char> || std::is_same_v<T,const wchar_t>);
		static constexpr size_t len() { return N; }
		static constexpr size_t Len = N;
		static constexpr bool Wide = std::is_same_v<T, wchar_t>;
		static constexpr size_t UnitSize = sizeof(T);
		std::array<T,N> data;
		constexpr ct_string(std::array<T, N> data) : data(data){}
		template<size_t I>
		constexpr T get(){ return data[I];} 
		T* get_data(){ return data.data(); }
	};

	template <typename S, std::size_t ...N>
	constexpr ct_string<std::remove_reference_t < decltype(S::get()[0])>,sizeof...(N)>
		prepare_ct_str_impl(S, std::index_sequence<N...>)
	{
		return { { { S::get()[N] ... } } };
	}


	template <typename S>
	constexpr decltype(auto) prepare_ct_str(S s) {
		return prepare_ct_str_impl(s,
			std::make_index_sequence< sizeof(S::get())>{});
	}

	/// <summary>
	/// int idx = eq_ct_str_ret(".vert",PREPARE_CT_STR(L".vert"), PREPARE_CT_STR(L".frag"), PREPARE_CT_STR(L".comp")); //0
	/// </summary>
	/// <typeparam name="F"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="p"></param>
	/// <param name="s"></param>
	/// <param name="...strs"></param>
	/// <returns></returns>
	template<size_t I, typename F, typename ...Args>
	size_t __eq_ct_str_ret(const wchar_t* p, F&& s, Args&&... strs)
	{
		if (wcscmp(p, s.get_data()) == 0)
		{
			return I;
		}
		else {
			if constexpr (sizeof...(Args) == 0)
			{
				return -1;
			}
			else {
				return __eq_ct_str_ret<I + 1>(p, std::forward<Args>(strs)...);
			}
		}
	}

	template<typename ...Args>
	size_t eq_ct_str_ret(const wchar_t* p, Args&&... strs)
	{
		if constexpr (sizeof...(Args) == 0)
		{
			return -1;
		}
		else {
			return __eq_ct_str_ret<0>(p, std::forward<Args>(strs)...);
		}
	}

	template<size_t I, typename F, typename ...Args>
	size_t __eq_ct_str_ret(const char* p, F&& s, Args&&... strs)
	{
		if (strcmp(p, s.get_data()) == 0)
		{
			return I;
		}
		else {
			if constexpr (sizeof...(Args) == 0)
			{
				return -1;
			}
			else {
				return __eq_ct_str_ret<I + 1>(p, std::forward<Args>(strs)...);
			}
		}
	}

	template<typename ...Args>
	size_t eq_ct_str_ret(const char* p, Args&&... strs)
	{
		if constexpr (sizeof...(Args) == 0)
		{
			return -1;
		}
		else {
			return __eq_ct_str_ret<0>(p, std::forward<Args>(strs)...);
		}
	}
}
#define PREPARE_STD_ARR(s)													\
    (::wws::prepare_std_arr([]{												\
        struct tmp {                                                        \
            static constexpr decltype(auto) get() { return s; }             \
        };                                                                  \
        return tmp{};                                                       \
    }())) 
#define  PREPARE_CT_STR(s) 													\
    (::wws::prepare_ct_str([]{												\
        struct tmp {                                                        \
            static constexpr decltype(auto) get() { return s; }             \
        };                                                                  \
        return tmp{};                                                       \
    }())) 

#endif 