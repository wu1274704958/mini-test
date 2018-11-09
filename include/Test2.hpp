#ifndef __WWS_TEST2_H__
#define __WWS_TEST2_H__

#include <iostream>
#include "TestFunc.hpp"
namespace test2{

void test2_1()
{
    std::cout << "void test2_1()" << std::endl;
}

auto init()
{
    return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test2_1)
	);
}

}// namespace test2

#endif //__WWS_TEST2_H__