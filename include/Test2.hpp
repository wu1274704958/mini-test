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

int add(int a,int b)
{
    std::cout << a + b <<std::endl;
    return a + b;
}

void test2_2()
{
    int a = 9;
    auto r = std::reference_wrapper<int>(a);
    int& d = r;
    int&& e = std::forward<int>(r);

    d = 98;
    e = 90;

    std::cout << a << std::endl;

    auto f = std::bind(add,a,a);
    f();
}

class Test{
public:
    Test(){
        printf("Test()\n");
    }
    ~Test(){
        printf("~Test()\n");
    }

};

void f(Test t){
    
}

void test2_3()
{
    try{
        throw std::runtime_error("sss");
    }catch(...)
    {
        printf("exception!\n");
    }
    f(Test());
}

auto init()
{
    return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test2_1),
                                    CREATE_TEST_FUNC(test2_2),
                                    CREATE_TEST_FUNC(test2_3)
	);
}

}// namespace test2

#endif //__WWS_TEST2_H__