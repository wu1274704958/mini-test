#pragma once
#include <random>
#include <functional>

namespace wws
{
    template<typename T,typename EG>
    requires requires(EG& e,std::uniform_int_distribution<size_t> dist)
    {
        requires std::is_same_v<size_t,decltype(dist(e))>; 
    }
    size_t randomUnique(EG& engine,const std::vector<T>& src,size_t begin,size_t end,size_t currentCount,size_t count,
        std::function<void(const T&)> out_func)
    {
        if (currentCount >= count || begin >= end)
            return currentCount;
        std::uniform_int_distribution<size_t> dist(begin,end - 1);
        const auto rand = dist(engine);
        if (out_func)
            out_func(src[rand]);
        ++currentCount;
        if (begin != rand)
            currentCount = randomUnique(engine,src,begin, rand,currentCount,count,out_func);
        if (end - 1 != rand)
            currentCount = randomUnique(engine,src,rand + 1, end,currentCount,count, out_func);
        return currentCount;
    }

    template<typename T>
    size_t randomUnique(const std::vector<T>& src,size_t count,std::function<void(const T&)> out_func,int seed = 0)
    {
        if (seed == 0)
        {
            std::random_device rd;
            std::mt19937 engine(rd());
            return randomUnique(engine,src, 0, src.size(),0, count, out_func);
        }else
        {
            std::mt19937 engine(seed);
            return randomUnique(engine,src, 0, src.size(),0, count, out_func);
        }
    }
};
