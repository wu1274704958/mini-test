#ifndef __WWS_TEST2_H__
#define __WWS_TEST2_H__

#include <iostream>
#include "TestFunc.hpp"
#include <functional>
#include <any>

namespace test2{
using std::cout;
using std::endl;

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
	int *p;
    Test(){
		p = new int(9);
        printf("Test()\n");
    }
	Test(Test&& t) {
		this->p = t.p;
		t.p = nullptr;
		printf("Test(Test&& )\n");
	}
	Test(const Test& t) {
		this->p = t.p;
		printf("Test(Test& )\n");
	}
    ~Test(){
		if (p)
			delete p;
        printf("~Test()\n");
    }

};

void f(Test t){
    
}

void test2_3()
{
    try{
		//int *p = reinterpret_cast<int*>(1);
		//*p = 9;
    }catch(...)
    {
        printf("exception!\n");
    }
    f(Test());
}
using std::any;
using std::any_cast;
void test_any()
{
    any a = 1;
    if(a.has_value())
    {
        printf("%s  %d\n",a.type().name(),any_cast<int>(a));
    }

    int *p = any_cast<int>(&a);
    cout << *p << endl;
}

auto init()
{
    return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test2_1),
                                    CREATE_TEST_FUNC(test2_2),
                                    CREATE_TEST_FUNC(test2_3),
                                    CREATE_TEST_FUNC(test_any)
	);
}

}// namespace test2

#endif //__WWS_TEST2_H__