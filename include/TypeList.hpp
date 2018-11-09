#ifndef __WWS_TYPE_LIST_H__
#define __WWS_TYPE_LIST_H__

namespace wws{
    template<typename ...Args>
    struct TypeList;

    template<typename Now,typename ...Args>
    struct TypeList<Now,Args...> : public TypeList<Args...>
    {
        using NowType = Now;
        using BaseType = TypeList<Args...>;
    };

    template<>
    struct TypeList<>
    {
        using NowType = void;
        using BaseType = void;
    };

    template<typename T,typename ...Args>
    constexpr auto TL_ADD(TypeList<Args...>&& tl,T&& t) -> TypeList<T,Args...>
    {
        return TypeList<Args...,T>();
    }

    template<typename T,typename ...Args>
    constexpr auto TL_SUB(TypeList<T,Args...>&& tl) -> TypeList<Args...>
    {
        return TypeList<Args...>();
    }

    template<typename TL,typename T>
    struct TypeAdd{
        using type = decltype( TL_ADD( std::declval<TL>(),std::declval<T>() ) );
    };

    template<typename TL>
    struct TypeSub{
        using type = decltype( TL_SUB( std::declval<TL>() ) );
    };
}


#endif //__WWS_TYPE_LIST_H__