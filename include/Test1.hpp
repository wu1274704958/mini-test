#ifndef __WWS_TEST1_H__
#define __WWS_TEST1_H__

#include <iostream>
#include <array>
#include "TypeList.hpp"
#include "TestFunc.hpp"

namespace test1{
	
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
struct Test{
	Test(){
		std::cout << "Test()" <<std::endl;
	}
	Test(const Test&){
		std::cout << "Test(Test&)" <<std::endl;
	}
	Test(Test &&){
		std::cout << "Test(Test &&)" <<std::endl;
	}
	~Test(){
		std::cout << "~Test()" <<std::endl;
	}
};
void test4(Test&& t)
{
	std::cout << "test4()" <<std::endl;
}
void test1()
{
	std::cout << std::is_same< wws::TypeAdd< wws::TypeList<int,char> , int >::type , wws::TypeList<int,int,char> >::value << "\n";
	std::cout << std::is_same< wws::TypeList<int,char>::BaseType::BaseType , wws::TypeList<> >::value << "\n";
	std::cout << std::is_same< wws::TypeSub< wws::TypeList<int,char> >::type , wws::TypeList<char> >::value << "\n";
}
void test2()
{
	std::cout << "test2()" <<std::endl;
}
void test3()
{
	std::cout << "test3()" <<std::endl;
}


decltype(auto) init()
{
	return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test1),
									CREATE_TEST_FUNC(test2),
									CREATE_TEST_FUNC(test)
	);
}

} // namespace test1

#endif //#ifndef __WWS_TEST1_H__