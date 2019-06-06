//
// Created by dell on 2019/6/6.
//
#include "print_stl.hpp"
#include <iostream>
#include <tuple>
#include <utility>
#include "TestFunc.hpp"
#include "TypeList.hpp"
#include <string>
#include <lc1.hpp>

int main(int argc,char **argv) {

    auto tup = wws::CatTFArray(lc1::init());
    int a = 0;

    while (a != -1)
    {
        std::cout << "\n---------------------------------------------------------------------------\n";
        wws::PrintFuncName(MAEK_IS_TUPLE_SIZE(tup),tup);
        std::cin >> a;
        std::cout << "---------------------------------------------------------------------------\n";
        wws::RunFuncNoArgs(MAEK_IS_TUPLE_SIZE(tup), tup, a);
        std::cout << std::endl;
    }
}