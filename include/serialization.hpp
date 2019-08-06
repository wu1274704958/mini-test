#pragma once
#include <iostream>
#include <string>
#include <cassert>

namespace wws {

	template<typename T,typename = std::enable_if_t< std::is_integral_v<T> || std::is_floating_point_v<T>>>
	T parser(const std::string& str, size_t _Base = 10)
	{
		if constexpr (std::is_same_v<int, T> || std::is_same_v<short,T> || std::is_same_v<char,T> )
		{
			return static_cast<T>(std::stoi(str,nullptr,_Base));
		}else 
			if constexpr (std::is_same_v< long, T>)
		{
			return std::stol(str,nullptr,_Base);
		}else
			if constexpr (std::is_same_v<long long, T>)
		{
			return std::stoll(str, nullptr, _Base);
		}else
			if constexpr (std::is_same_v<float, T>)
		{
			return std::stof(str);
		}else
			if constexpr (std::is_same_v<double, T>)
		{
			return std::stod(str);
		}else
			if constexpr (std::is_same_v<long double, T>)
		{
			return std::stold(str);
		}else
			if constexpr (	std::is_same_v<unsigned int, T> || 
							std::is_same_v<unsigned short,T> || 
							std::is_same_v<unsigned char, T> || 
							std::is_same_v<unsigned long,T>)
		{
			return static_cast<T>( std::stoul(str));
		}else
			if constexpr (std::is_same_v<unsigned long long, T> )
		{
			return std::stoull(str);
		}
	}


	template<typename T,template <typename T1, typename Alloc = std::allocator<T1>> class C>
	void parser_stl(const std::string& str,C<T>& res)
	{
		int b = 0;
		for (int i = 0; i < str.size(); ++i)
		{
			if (str[i] == '[') b += 1;
			if (str[i] == ',' || str[i] == ']')
			{
				std::string temp;
				if (std::is_same_v<std::string, std::remove_cv_t<T>> && (str[b] == '"' || str[b] == '\'') )
				{
					temp = str.substr(b + 1, (i - (b + 1)) - 1);
				}
				else {
					temp = str.substr(b, i - b);
				}
				
				if (temp.empty()) continue;
				if constexpr (std::is_same_v<std::string, std::remove_cv_t<T>>)
				{
					res.push_back(std::move(temp));
				}
				else {
					res.push_back(parser<T>(temp));
				}
				b = i + 1;
			}
		}
	}

	template<typename T>
	struct is_std_list {
		static constexpr bool val = false;
	};

	template<typename T,template <typename T1, typename Alloc = std::allocator<T1>> class C>
	struct is_std_list<C<T>>
	{
		static constexpr bool val = true;
	};


	template<typename T, typename = std::enable_if_t<std::is_same_v<std::string, decltype(std::to_string(std::declval<T>()))>>>
	std::string to_string(T t)
	{
		return std::to_string(t);
	}

	template<typename T,template <typename T1,typename Alloc = std::allocator<T1>> class C>
	std::string to_string(C<T>& c,bool has_qm = false)
	{
		std::string res;
		res += "[";
		size_t i = 0;
		for (auto& n : c)
		{
			if constexpr (std::is_same_v<std::string, std::remove_cv_t<T>>)
			{
				if (has_qm) res += '"';
				res += n;
				if (has_qm) res += '"';
			}
			else {
				res += to_string(n);
			}
			if (i != (c.size() - 1))
			{
				res += ',';
			}
			++i;
		}
		res += "]";
		return res;
	}

	template<typename CLS,typename F , typename ...Chs>
	void seilza_to_line_sub(std::string& res,CLS& cls,F CLS::* first, Chs CLS::* ... chs)
	{
		if constexpr (std::is_same_v<std::string, std::remove_cv_t<F>>)
		{
			res += cls.*first;
		}
		else {
			res += to_string(cls.*first);
		}
		
		if constexpr (sizeof...(chs) > 0)
		{
			res += '\t';
			seilza_to_line_sub(res, cls, chs ...);
		}
	}

	template<typename CLS,typename ...Chs>
	std::string seilza_to_line(CLS& cls,Chs CLS::* ... chs)
	{
		std::string res;
		if constexpr (sizeof...(chs) > 0)
		{
			seilza_to_line_sub(res, cls, chs ...);
		}
		res += '\n';
		return res;
	}

	template<typename CLS, typename F, typename ...Chs>
	void seilza_form_line_sub(const std::vector<std::string> ceils,uint32_t index, CLS& cls, F CLS::* first, Chs CLS::* ... chs)
	{
		if constexpr (std::is_same_v<std::string, std::remove_cv_t<F>>)
		{
			cls.*first = std::move(ceils[index]); 
		}
		else if constexpr (is_std_list<F>::val) {
			F c;
			parser_stl(ceils[index],c);
			cls.*first = std::move(c);
		}
		else{
			cls.*first = parser<F>(ceils[index]);
		}
		if constexpr (sizeof...(chs) > 0)
		{
			seilza_form_line_sub(ceils, index + 1,cls, chs ...);
		}
	}

	template<typename CLS, typename ...Chs>
	void seilza_form_line(const std::string& str,CLS& cls, Chs CLS::* ... chs)
	{
		std::vector<std::string> ceils;
		int b = 0;
		for (int i = 0; i < str.size(); ++i)
		{
			if (str[i] == '\t' || str[i] == '\n')
			{
				ceils.push_back(str.substr(b, i - b));
				b = i + 1;
			}
		}

		assert(ceils.size() >= sizeof...(chs));
		if constexpr (sizeof...(chs) > 0)
		{
			seilza_form_line_sub(ceils, 0, cls, chs ...);
		}
	}

}


