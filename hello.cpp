#include <iostream>
#include <string_view>
#include <array>
#include <tuple>
#include <utility>
#include "TestFunc.hpp"
#include "TypeList.hpp"
#include "Test1.hpp"
#include "Test2.hpp"
#include "Test3.hpp"
#include <string>
#include "print_stl.hpp"


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
	auto tup = wws::CatTFArray(test1::init(),test2::init(),test3::init());
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
			RUN_TF_CASE_E(13, tup, s, 3)

			RUN_TF_CASE(14, tup)
			RUN_TF_CASE(15, tup)
			RUN_TF_CASE(16, tup)

			RUN_TF_CASE(17, tup, "MCMXCIV")

			RUN_TF_CASE_B(18)
				std::vector<std::string> strs = {
					"flower","flow","flight"
				};
			RUN_TF_CASE_E(18, tup, strs)

			RUN_TF_CASE_B(19)
				std::vector<int> in = { 3,0,-2,-1,1,2 };
			RUN_TF_CASE_E(19, tup, in)

			RUN_TF_CASE(20, tup, "23");

			RUN_TF_CASE_B(21)
				std::vector<int> in = { -3,-2,-1,0,0,1,2,3 };
			RUN_TF_CASE_E(21, tup, in,0)
			
			RUN_TF_CASE(22, tup);
		}
	}
	return 0;
}
