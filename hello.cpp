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
			case 0: wws::RunFuncIndex<0>(tup); break;
			case 1: wws::RunFuncIndex<1>(tup); break;
			case 2: wws::RunFuncIndex<2>(tup,argc,argv); break;
			case 3: wws::RunFuncIndex<3>(tup,test1::Test()); break;
			case 4: {
				test1::Test t;
				wws::RunFuncIndex<4>(tup,t); break;
			}
			case 5: wws::RunFuncIndex<5>(tup); break;
			case 6: wws::RunFuncIndex<6>(tup); break;
			case 7: wws::RunFuncIndex<7>(tup); break;
		}
		std::cout << "\n\n" ;
	}
	return 0;
}
