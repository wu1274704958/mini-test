#include <iostream>
#include "tool.h"
#define BOOST_TEST_MODULE ToolTests
#include <boost/test/unit_test.hpp>
#include "HasMember.hpp"



struct Struct{
    int a;
    long b;
    double c;
};

HAS_MEMBER(a)
HAS_MEMBER(d)

BOOST_AUTO_TEST_SUITE(test_suite)

BOOST_AUTO_TEST_CASE(FailTest)
{
    BOOST_CHECK_EQUAL(9, req(3));
}
 
 
BOOST_AUTO_TEST_CASE(PassTest)
{
    BOOST_CHECK_EQUAL(4, req(2));
}

BOOST_AUTO_TEST_CASE(my_offset_test)
{
    BOOST_CHECK_EQUAL(0,m_offsetof(Struct,a));
    BOOST_CHECK_EQUAL(8,m_offsetof(Struct,b));
    BOOST_CHECK_EQUAL(16,m_offsetof(Struct,c));
}

BOOST_AUTO_TEST_CASE(my_has_member_test)
{
    BOOST_CHECK_EQUAL( true,wws::Hasa<Struct>::value );
    BOOST_CHECK_EQUAL( false,wws::Hasd<Struct>::value );
}

BOOST_AUTO_TEST_SUITE_END()

#include <boost/assign.hpp>

using namespace std;

struct global_fixture // 全局测试夹具类
{
    global_fixture(){cout << ("global setup\n");}
    ~global_fixture(){cout << ("global teardown\n");}
};

// 该宏定义的夹具类被应用于整个测试用例的所有测试套件, 包括主测试套件
BOOST_GLOBAL_FIXTURE(global_fixture);

struct assign_fixture // 测试套件夹具类
{
    assign_fixture()
    {cout << ("suit setup\n");}
    ~assign_fixture()
    {cout << ("suit teardown\n");}

    vector<int> v;    // 所有测试用例都可以用的成员变量
};

// 定义测试套件级别的夹具
BOOST_FIXTURE_TEST_SUITE(s_assign, assign_fixture)

BOOST_AUTO_TEST_CASE(t_assign1) // 测试用例1, 测试+=操作符
{
    using namespace boost::assign;

    v += 1,2,3,4;
    BOOST_CHECK_EQUAL(v.size(), 4);
    BOOST_CHECK_EQUAL(v[2], 3);
}

BOOST_AUTO_TEST_CASE(t_assign2) // 测试用例2, 测试push_back函数
{
    using namespace boost::assign;

    push_back(v)(10)(20)(30);

    BOOST_CHECK_EQUAL(v.empty(), false);
    BOOST_CHECK_LT(v[0], v[1]);
}

BOOST_AUTO_TEST_SUITE_END() // 测试套件结束
