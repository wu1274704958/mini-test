#pragma once


#include <dbg.hpp>
#include <TestFunc.hpp>
#include <vector>
#include <typeinfo>
#include <print_stl.hpp>

namespace emcpp_chapter_1 {

	template<typename T>
	std::vector<T> create_vec()
	{
		

		std::vector<T> v{7,8,9};
		return v;
	}

	template <typename T>	 
	void f(const T&  param) // 右值无法赋值给左值引用
	{
		const int* const p = nullptr;
		dbg(typeid(p).name());
		
		dbg(typeid(T).name());
		//(*param) = 9;
	}

	void page_37() {

		const auto v = create_vec<int>();
		dbg(v);
		f(&v[0]); // T is const int * // param is const int* const & 
		dbg(v);		
	}
	

	auto init() {
		return wws::CreateTFArray(
			CREATE_TEST_FUNC(page_37)
		);
	}
}