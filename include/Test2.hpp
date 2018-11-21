#ifndef __WWS_TEST2_H__
#define __WWS_TEST2_H__

#include <iostream>
#include "TestFunc.hpp"
#include <functional>

namespace test2{


template<int N, int... Indexes>
struct IndexTuple {
    using type = typename IndexTuple<N - 1,N - 1,Indexes...>::type;
};// 3 -> 2 2 -> 1 1 2 -> 0 0 1 2 -> 0 1 2       

template<int... indexes>
struct IndexTuple<0, indexes...>
{
    typedef IndexTuple<indexes...> type;
};
template<int I>
struct make_index{
    using type = typename IndexTuple<I>::type;
};

template<int ...Argc>
void print_index_tuple(IndexTuple<Argc ...> it)
{
    ((std::cout << Argc << " "),...); 
}

void test2_1()
{
    using type = make_index<7>::type;
    print_index_tuple(type());
}

void t(test1::Test &t)
{
	std::cout << " void t() func run" << std::endl;
	test1::Test t2 = std::move(t);
}

void test_cv()
{
	using namespace std::placeholders;
	int a;
	auto r = std::reference_wrapper(a);
	int &c = r;
	test1::Test t3;
	auto f = std::bind(t,std::ref(t3));
	f();
}

auto init()
{
    return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test2_1),
									CREATE_TEST_FUNC(test_cv)
	);
}

}// namespace test2

#endif //__WWS_TEST2_H__