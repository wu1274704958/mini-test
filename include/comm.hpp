#pragma once
#include <iostream>
#include <functional>
#include <optional>

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

		if (temp._1 == static_cast<short>(0x1122))
			return true;
		else
			return false;
	}

	template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
	T reverse_byte(T t)
	{
		int len = static_cast<int>(sizeof(T));
		char* p = reinterpret_cast<char*>(&t);
		int e = len - 1;
		int b = 0;
		while (b < e)
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

	///////////////////////////////////vkd////////////////////////////////////////////
	inline bool IsContainEq__(const char* a, const char* b)
	{
		return strcmp(a, b) == 0;
	}

	template<typename V, template <typename T1, typename Alloc = std::allocator<T1>> class T>
	bool IsContain(const T<V>& vec, const T<V>& inclusion)
	{
		int same_count = 0;
		for (auto& i : inclusion)
		{
			bool has = false;
			for (auto& v : vec)
			{
				if (IsContainEq__(i, v))
				{
					has = true;
					++same_count;
					break;
				}
			}
			if (same_count == inclusion.size()) return true;
			if (!has) return false;
		}
		return true;
	}

	template<typename V, typename V1, template <typename T1, typename Alloc = std::allocator<T1>> class T>
	bool IsContain(const T<V1>& vec, const T<V>& inclusion, V V1::* f)
	{
		int same_count = 0;
		for (auto& i : inclusion)
		{
			bool has = false;
			for (auto& v : vec)
			{
				if (IsContainEq__(i, v.*f))
				{
					has = true;
					++same_count;
					break;
				}
			}
			if (same_count == inclusion.size()) return true;
			if (!has) return false;
		}
		return true;
	}

	template<typename V, typename V1, template <typename T1, typename Alloc = std::allocator<T1>> class T>
	bool IsContain(const T<V1>& vec, const T<V>& inclusion, std::function<V(const V1&)> f)
	{
		int same_count = 0;
		for (auto& i : inclusion)
		{
			bool has = false;
			for (auto& v : vec)
			{
				if (IsContainEq__(i, f(v)))
				{
					has = true;
					++same_count;
					break;
				}
			}
			if (same_count == inclusion.size()) return true;
			if (!has) return false;
		}
		return true;
	}

	template<typename T, T F, T ...N>
	bool eq_enum(T n)
	{
		if constexpr (sizeof...(N) == 0)
		{
			return n == F;
		}
		else {
			return n == F || eq_enum<T, N...>(n);
		}
	}

	template<typename T, typename ...Args>
	struct VarCache;

	template<typename T>
	struct VarCache<T, std::tuple<>>
	{
	public:
		using INType = std::tuple<>;
		VarCache() {}
		VarCache(std::function<T()> f) {
			constructor = f;
		}
		operator std::shared_ptr<T>()
		{
			if (!ptr)
			{
				ptr = std::shared_ptr(new T(constructor()));
			}
			return ptr;
		}
		void clear()
		{
			ptr.reset();
		}
	private:
		std::function<T()> constructor;
		std::shared_ptr<T> ptr;
	};

	template<typename T, typename ...Args>
	struct VarCache
	{
	public:
		using INType = std::tuple<Args&...>;
		VarCache() {}
		VarCache(std::function<T(INType&)> f, Args&...args) : constructor(f), in(std::forward<Args&>(args)...) {
		}
		operator std::shared_ptr<T>()
		{
			if (!ptr && in.has_value())
			{
				ptr = std::shared_ptr<T>(new T(constructor(in.value())));
			}
			return ptr;
		}
		void clear()
		{
			ptr.reset();
		}
	private:
		std::function<T(INType&)> constructor;
		std::shared_ptr<T> ptr;
		std::optional<INType> in;
	};

	template<typename T, T...Ts>
	struct ValList
	{
		using Type = T;
		static constexpr size_t Len = sizeof...(Ts);
		template<size_t I>
		static constexpr T get()
		{
			return get_<0, I, T, Ts...>();
		}
		static std::optional<T> get(size_t i)
		{
			return get_<0, T, Ts...>(i);
		}
		template<T K>
		static constexpr size_t find()
		{
			return find_<0, K, Ts...>();
		}
		static int find(T t)
		{
			return find_<0, T, Ts...>(t);
		}

	private:
		template<size_t I, size_t C, typename TT, TT F, TT ...S>
		static constexpr T get_()
		{
			if constexpr (I == C)
			{
				return F;
			}
			else
			{
				static_assert(sizeof...(S) > 0, "Not Found!!!");
				return get_<I + 1, C, TT, S...>();
			}
		}

		template<size_t I, typename TT, TT F, TT ...S>
		static std::optional<T> get_(size_t c)
		{
			if (I == c)
			{
				return F;
			}
			else
			{
				if constexpr (sizeof...(S) == 0)
				{
					return std::nullopt;
				}
				else {
					return get_<I + 1, TT, S...>(c);
				}
			}
		}
		template<size_t I, typename TT, TT F, TT ...S>
		inline static int find_(T t)
		{
			if (F == t)
			{
				return I;
			}
			else
			{
				if constexpr (sizeof...(S) == 0)
				{
					return -1;
				}
				else {
					return find_<I + 1, TT, S...>(t);
				}
			}
		}
		template<size_t I, T K, T F, T ...S>
		static constexpr size_t find_()
		{
			if constexpr (F == K)
			{
				return I;
			}
			else
			{
				if constexpr (sizeof...(S) == 0)
				{
					return -1;
				}
				else {
					return find_<I + 1, K, S...>();
				}
			}
		}
	};

	template<typename T, typename Tup, size_t N = 0>
	struct find_tuple
	{
		static constexpr int64_t func()
		{
			if constexpr (std::is_same_v<T, std::tuple_element_t<N, Tup>>)
			{
				return N;
			}
			else
			{
				if constexpr (N + 1 >= std::tuple_size<Tup>::value)
				{
					return -1;
				}
				else
				{
					return find_tuple<T, Tup, N + 1>::func();
				}
			}
		}
		static constexpr int64_t val = func();
	};

	template<typename T, typename E, typename Tup>
	constexpr std::optional<typename E::Type> map_enum()
	{
		constexpr int64_t idx = find_tuple<T, Tup>::val;
		if constexpr (idx >= 0)
		{
			return E::template get<idx>();
		}
		else
		{
			return std::nullopt;
		}
	}

	template<typename T, typename T2>
	std::optional<typename T2::Type> map_enum(typename T::Type t)
	{
		int idx = T::find(t);
		if (idx >= 0)
		{
			return T2::get((size_t)idx);
		}
		else {
			return std::nullopt;
		}
	}
	template<char Ap = '/'>
	bool up_path(std::string& res)
	{
		if (res.empty()) return false;
		while (res.back() != Ap)
		{
			if (res.empty()) return false;
			res.pop_back();
		}
		res.pop_back();
		return true;
	}

	template<char Ap = '/'>
	std::optional<std::string> append_path(const std::string& path, const char* str)
	{
		if (str[0] == '\0' || (str[1] == '\0' && str[0] == '.')) return path;
		std::string res = path;
		int b = 0;
		int i = 0;
		for (;;)
		{
			if (str[i] == '\0') break;
			if (str[i] == Ap)
			{
				if (i == b) { ++i; continue; }
				auto c = i - b;
				if (c == 1 && str[b] == '.') { b = i + 1; ++i; continue; }
				if (c == 2 && str[b] == '.' && str[b + 1] == '.')
				{
					if (!up_path(res))return std::nullopt;
					b = i + 1; ++i; continue;
				}
				res += Ap;
				res.append(str + b, i - b);
				b = i + 1;
			}
			++i;
		}
		if (str[b] != '\0')
		{
			auto c = i - b;
			if (c == 1 && str[b] == '.')  return res;
			if (c == 2 && str[b] == '.' && str[b + 1] == '.')
			{
				if (!up_path(res))return std::nullopt;
				return res;
			}
			res += Ap;
			res.append(str + b, i - b);
		}
		return res;
	}
	///////////////////////////////////vkd////////////////////////////////////////////
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

	template<typename T, size_t N>
	struct ct_string {
		static_assert(std::is_same_v<T, const char> || std::is_same_v<T, const wchar_t>);
		static constexpr size_t len() { return N; }
		static constexpr size_t Len = N;
		static constexpr bool Wide = std::is_same_v<T, wchar_t>;
		static constexpr size_t UnitSize = sizeof(T);
		std::array<T, N> data;
		constexpr ct_string(std::array<T, N> data) : data(data) {}
		template<size_t I>
		constexpr T get() { return data[I]; }
		T* get_data() { return data.data(); }
	};

	template <typename S, std::size_t ...N>
	constexpr ct_string<std::remove_reference_t < decltype(S::get()[0])>, sizeof...(N)>
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