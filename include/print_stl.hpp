#pragma once
#include <iostream>

template<typename V,template <typename T1,typename Alloc = std::allocator<T1>> class T>
std::ostream& operator<<(std::ostream& out,T<V>& v) 
{     
	out << "[ ";
	size_t i = 0;
	for(auto& n : v)
	{         
		if(i == (v.size() - 1))
			out << n;
		else             
			out << n << ',';
		++i;     
	}     
	out << " ]";
	return out; 
}
