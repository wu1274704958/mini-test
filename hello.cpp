#include <iostream>
#include <string_view>
#include <array>
#include <tool.h>
#include <tuple>
#include <utility>
#include "TestFunc.hpp"
#include "TypeList.hpp"
#include <functional>

int test(int argc,char **argv)
{
	std::array<char,100> arr;
	arr.fill(0);
	std::string_view sv("2018 10 31",4);

	if(argc > 1)
	{
		snprintf(arr.data(),arr.size() - 1,"HelloWorld %s %s\n",sv.data(),argv[1]);
	}else if(argc == 1){
		snprintf(arr.data(),arr.size() - 1,"HelloWorld %s\n",sv.data());
	}
	std::cout << arr.data() << std::endl;
}
// struct Test{
// 	Test(){
// 		std::cout << "Test()" <<std::endl;
// 	}
// 	Test(const Test&){
// 		std::cout << "Test(Test&)" <<std::endl;
// 	}
// 	Test(Test &&){
// 		std::cout << "Test(Test &&)" <<std::endl;
// 	}
// 	~Test(){
// 		std::cout << "~Test()" <<std::endl;
// 	}
// };
// void test4(Test&& t)
// {
// 	std::cout << "test4()" <<std::endl;
// }
void test1()
{
	std::cout << std::is_same< wws::TypeAdd< wws::TypeList<int,char> , int >::type , wws::TypeList<int,int,char> >::value << "\n";
	std::cout << std::is_same< wws::TypeList<int,char>::BaseType::BaseType , wws::TypeList<> >::value << "\n";
	std::cout << std::is_same< wws::TypeSub< wws::TypeList<int,char> >::type , wws::TypeList<char> >::value << "\n";
}
void test2()
{

}
void test3()
{
	std::cout << "test3()" <<std::endl;
}

int main(int argc,char **argv)
{
	
	wws::FuncInfo<decltype(test)> f(test);
	f(argc,argv);
	auto tup = wws::CreateTFArray(CREATE_TEST_FUNC(test1),CREATE_TEST_FUNC(test2));
	wws::PrintFuncName(MAEK_IS_TUPLE_SIZE(tup),tup);
	int a;
	std::cin >> a;
	wws::RunFuncIndex(MAEK_IS_TUPLE_SIZE(tup),tup,a);

	return 0;
}
