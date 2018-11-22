#ifndef __WWS_TEST_FUNC_H__
#define __WWS_TEST_FUNC_H__

#include <iostream>
#include "HasMember.hpp"
#include "TypeList.hpp"

#define CREATE_TEST_FUNC(func_name) wws::TestFunc(#func_name ,func_name)
#define MAEK_IS_TUPLE_SIZE(tup) std::make_index_sequence<std::tuple_size_v< std::remove_reference_t< decltype(tup) > >>()

#define RUN_TF_CASE(i,tup,...)  case i: wws::RunFuncIndex<i>(tup,##__VA_ARGS__); break;
#define RUN_TF_CASE_B(i)  case i: {  
#define RUN_TF_CASE_E(i,tup,...)  wws::RunFuncIndex<i>(tup,##__VA_ARGS__); break;}

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
        FuncType mFunc;
        FuncInfo(FuncType func):mFunc(func){}
//        template<typename ..._Args>
        RetType operator()(Args...args)
        {
            mFunc(std::forward<Args>(args)...);
        }
    };

    template<typename Ret,typename ...Args>
    struct FuncInfo<Ret(*)(Args...)>
    {
        using RetType = Ret;
        using AgrsType = TypeList<Args...>;
        using FuncType = Ret(*)(Args...);
        FuncType mFunc;
        FuncInfo(FuncType func):mFunc(func){}
        template<typename ..._Args>
        RetType operator()(_Args&&...args)
        {
            return mFunc(std::forward<Args>(args)...);
        }
    };
    
    template<typename Tup,size_t ...Idx>
    constexpr bool IsTestFuncTuple()
    {
        bool res = (wws::Hasname< std::remove_reference_t<  decltype( std::get<Idx>( std::declval<Tup>() ) ) > >::value && ...);
        return res;
    }

    template<typename ...Args>
    auto CreateTFArray(Args&& ...args)
    {
    	return std::make_tuple(args ...);
    }

    template<typename ...Tups>
    auto CatTFArray(Tups&& ...t)
    {
        return std::tuple_cat(std::forward<Tups>(t)...);
    }

    template<typename TUP,size_t ...Idx>
    void PrintFuncName(std::index_sequence<Idx...> is,TUP &tup)
    {
        static_assert( IsTestFuncTuple<TUP,Idx...>() ,"The elements in this tuple do not meet the requirements!");
    	((printf("%d: %s\n",Idx,std::get<Idx>(tup).name),...));
    }

    template<size_t I,typename TUP>
    auto GetFuncIndex(TUP &tup) -> FuncInfo< decltype(std::get<I>(tup).func) > 
    {
        static_assert( wws::Hasfunc< std::remove_reference_t<  decltype( std::get<I>(tup) ) > >::value ,"The element in this tuple do not meet the requirements!");
        auto res = std::get<I>(tup).func;
        return FuncInfo<decltype(res)>(res);
    }


    template<size_t I,typename TUP,typename ...Args>
    auto RunFuncIndex(TUP &tup,Args&&...args) ->  typename FuncInfo< decltype(std::get<I>(tup).func) >::RetType
    {
        static_assert( 
            wws::Hasfunc< std::remove_reference_t<  decltype( std::get<I>(tup) ) > >::value && 
            wws::Hasname< std::remove_reference_t<  decltype( std::get<I>(tup) ) > >::value ,
            "The element in this tuple do not meet the requirements!");
        std::cout << std::get<I>(tup).name << " :"<< std::endl;    
        auto res = std::get<I>(tup).func;
        FuncInfo<decltype(res)> f(res);
        return f(std::forward<Args>(args)...);
    }
} // wws

#endif //__WWS_TEST_FUNC_H__