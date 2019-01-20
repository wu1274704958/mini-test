#ifndef __WWS_TEST1_H__
#define __WWS_TEST1_H__

#include <iostream>
#include <array>
#include "TypeList.hpp"
#include "TestFunc.hpp"
//#include <string_view>

namespace test1{
	
int test(int argc,char **argv)
{
	std::array<char,100> arr;
	arr.fill(0);
	//std::string_view sv("2018 10 31",4);
	const char *sv = "2018 10 31";

	if(argc > 1)
	{
		snprintf(arr.data(),arr.size() - 1,"HelloWorld %s %s\n",sv,argv[1]);
	}else if(argc == 1){
		snprintf(arr.data(),arr.size() - 1,"HelloWorld %s\n",sv);
	}
	std::cout << arr.data() << std::endl;
	return 0;
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
	Test t2 = std::move(t);
}
void test1()
{
	std::cout << std::is_same< wws::TypeAdd< wws::TypeList<int,char> , int >::type , wws::TypeList<int,int,char> >::value << "\n";
	std::cout << std::is_same< wws::TypeList<int,char>::BaseType::BaseType , wws::TypeList<> >::value << "\n";
	std::cout << std::is_same< wws::TypeSub< wws::TypeList<int,char> >::type , wws::TypeList<char> >::value << "\n";
}

void t2_2(Test &&t)
{
	Test newt = std::move(t);
}
void t2_1(Test &&t)
{
	printf("__func__ = %s\n",__func__);
	t2_2(std::move(t));
}

void test2()
{   
	Test t;
	t2_1(std::move(t));
}
void test3(Test& t)
{
	Test t2 = t;
}


decltype(auto) init()
{
	return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test1),
									CREATE_TEST_FUNC(test2),
									CREATE_TEST_FUNC(test),
									CREATE_TEST_FUNC(test4),
									CREATE_TEST_FUNC(test3)
	);
}

} // namespace test1

#endif //#ifndef __WWS_TEST1_H__