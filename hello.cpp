#include <iostream>
#include <string_view>
#include <array>
#include <tuple>
#include <utility>
#include "TestFunc.hpp"
#include "TypeList.hpp"
#include "Test1.hpp"
#include "Test2.hpp"
#include <string>

int main(int argc,char **argv)
{
	// wws::FuncInfo<decltype(test)> f(test);
	// f(argc,argv);
	
	
	
	/*
	//没有参数的测试用例
	auto tup = wws::CatTFArray(test1::init(),test2::init());
	int a = 0;
	wws::PrintFuncName(MAEK_IS_TUPLE_SIZE(tup), tup);
	while (a != -1)
	{
		std::cin >> a;
		wws::RunFuncNoArgs(MAEK_IS_TUPLE_SIZE(tup), tup, a);
		std::cout << "\r\n";
	}*/
	// auto f2 = wws::GetFuncIndex<0>(tup);
	// f2();
	// wws::RunFuncIndex<1>(tup);
	// wws::RunFuncIndex<2>(tup,argc,argv);
	//有参数的测试用例
	auto tup = wws::CatTFArray(test1::init(),test2::init());
	int a = 0;
	while(a > -1)
	{
		std::cout << "\n---------------------------------------------------------------------------\n";
		wws::PrintFuncName(MAEK_IS_TUPLE_SIZE(tup),tup);
		std::cin >> a;
		std::cout << "---------------------------------------------------------------------------\n";
		switch(a)
		{
			RUN_TF_CASE(0,tup)
			RUN_TF_CASE(1,tup)
			RUN_TF_CASE(2,tup,argc,argv)
			RUN_TF_CASE(3,tup,test1::Test())

			RUN_TF_CASE_B(4)
				test1::Test t;
			RUN_TF_CASE_E(4, tup, t)

			RUN_TF_CASE(5, tup)
			RUN_TF_CASE(6, tup)
			RUN_TF_CASE(7, tup)
			RUN_TF_CASE(8, tup)
			RUN_TF_CASE(9, tup)
			RUN_TF_CASE_B(10)
				std::vector<int> arr1 = { 1, 2, 79, 90 };
				
				std::vector<int> arr2 = { 3, 4, 67, 89, 100 };
			RUN_TF_CASE_E(10, tup, arr1,arr2)

			RUN_TF_CASE_B(11)
				std::vector<int> arr2 = { 4, 5 };

				std::vector<int> arr1 = { 2, 3};
			RUN_TF_CASE_E(11, tup, arr1, arr2)

			RUN_TF_CASE_B(12)

					std::string s = "cca";
			RUN_TF_CASE_E(12, tup, s)

			RUN_TF_CASE_B(13)

				std::string s = "PAYPALISHIRING";
			RUN_TF_CASE_E(13, tup, s, 3);

		}
	}
	return 0;
}
