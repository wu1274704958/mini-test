#include <iostream>
#include <string_view>
#include <array>
#include <tool.h>
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
	wws::PrintFuncName(MAEK_IS_TUPLE_SIZE(tup),tup);
	// auto f2 = wws::GetFuncIndex<0>(tup);
	// f2();
	// wws::RunFuncIndex<1>(tup);
	// wws::RunFuncIndex<2>(tup,argc,argv);
	int a = 0;
	while(a > -1)
	{
		std::cin >> a;
		switch(a)
		{
			case 0: wws::RunFuncIndex<0>(tup); break;
			case 1: wws::RunFuncIndex<1>(tup); break;
			case 2: wws::RunFuncIndex<2>(tup,argc,argv); break;
			case 3: wws::RunFuncIndex<3>(tup); break;
		}
	}
	return 0;
}
