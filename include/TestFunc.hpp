#ifndef __WWS_TEST_FUNC_H__
#define __WWS_TEST_FUNC_H__

#include <iostream>
#include "HasMember.hpp"
#include "TypeList.hpp"

#define CREATE_TEST_FUNC(func_name) wws::TestFunc(#func_name ,func_name)
#define MAEK_IS_TUPLE_SIZE(tup) std::make_index_sequence<std::tuple_size_v< std::remove_reference_t< decltype(tup) > >>()

HAS_MEMBER(name)
HAS_MEMBER(func)

namespace wws
{
    template<typename F>
    struct TestFunc{
    	const char *name;
    	F func;
        using FuncType = F;

    	TestFunc(const char *_name,F _func) : 
    	name(_name),
    	func(_func) 
    	{}
    };

    template<typename Ret,typename ...Args>
    struct FuncInfo;

    template<typename Ret,typename ...Args>
    struct FuncInfo<Ret(Args...)>
    {
        using RetType = Ret;
        using AgrsType = TypeList<Args...>;
        using FuncType = Ret(*)(Args...);
    };
    
    template<typename Tup,size_t ...Idx>
    constexpr bool IsTestFuncTuple(Tup &tup,std::index_sequence<Idx...> is)
    {
        bool res = (wws::Hasname< std::remove_reference_t<  decltype( std::get<Idx>(tup) ) > >::value && ...);
        return res;
    }

    template<typename ...Args>
    auto CreateTFArray(Args&& ...args)
    {
    	return std::make_tuple(args ...);
    }

    template<typename TUP,size_t ...Idx>
    void PrintFuncName(std::index_sequence<Idx...> is,TUP &tup)
    {
        static_assert( IsTestFuncTuple(std::forward<TUP&>(tup), is) ,"The elements in this tuple do not meet the requirements!");
    	((printf("%d: %s\n",Idx,std::get<Idx>(tup).name),...));
    }

    template<typename TUP,size_t ...Idx>
    void RunFuncIndex(std::index_sequence<Idx...> is,TUP &tup,int index)
    {
        static_assert( IsTestFuncTuple(std::forward<TUP&>(tup), is) ,"The elements in this tuple do not meet the requirements!");
    	((Idx == index && (printf("%s :\n",std::get<Idx>(tup).name),std::get<Idx>(tup).func(),false)),...);
    }
} // wws

#endif //__WWS_TEST_FUNC_H__