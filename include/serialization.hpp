#pragma once
#include <iostream>
#include <string>

namespace wws {

	template<typename T,typename = std::enable_if_t< std::is_integral_v<T> || std::is_floating_point_v<T>>>
	T parser(std::string& str, size_t _Base = 10)
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

}