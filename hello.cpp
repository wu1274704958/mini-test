#include <iostream>
#include <string_view>
#include <array>
#include <tuple>
#include <utility>
#include "TestFunc.hpp"
#include "TypeList.hpp"
#include "Test1.hpp"
#include "Test2.hpp"


int main(int argc,char **argv)
{
	// wws::FuncInfo<decltype(test)> f(test);
	// f(argc,argv);
	auto tup = wws::CatTFArray(test1::init(),test2::init());
	
	// auto f2 = wws::GetFuncIndex<0>(tup);
	// f2();
	// wws::RunFuncIndex<1>(tup);
	// wws::RunFuncIndex<2>(tup,argc,argv);
	int a = 0;
	while(a > -1)
	{
		wws::PrintFuncName(MAEK_IS_TUPLE_SIZE(tup),tup);
		std::cin >> a;
		switch(a)
		{
			RUN_TF_CASE(0,tup)
			RUN_TF_CASE(1,tup)
			RUN_TF_CASE(2,tup,argc,argv)
			RUN_TF_CASE(3,tup,test1::Test())

			RUN_TF_CASE_B(4)
				test1::Test t;
			RUN_TF_CASE_E(4,tup,t)

			RUN_TF_CASE(5,tup)
			RUN_TF_CASE(6,tup)
			RUN_TF_CASE(7,tup)
			RUN_TF_CASE(8,tup)
			RUN_TF_CASE(9, tup)
		}
		std::cout << "\n\n" ;
	}
	return 0;
}
