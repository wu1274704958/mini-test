#ifndef __WWS_TEST2_H__
#define __WWS_TEST2_H__

#include <iostream>
#include "TestFunc.hpp"
#include <functional>
#include <any>
#include <vector>

namespace test2{
using std::cout;
using std::endl;

template<int N, int... Indexes>
struct IndexTuple {
    using type = typename IndexTuple<N - 1,N - 1,Indexes...>::type;
};// 3 -> 2 2 -> 1 1 2 -> 0 0 1 2 -> 0 1 2       

template<int... indexes>
struct IndexTuple<0, indexes...>
{
    typedef IndexTuple<indexes...> type;
};
template<int I>
struct make_index{
    using type = typename IndexTuple<I>::type;
};

template<int ...Argc>
void print_index_tuple(IndexTuple<Argc ...> it)
{
    ((std::cout << Argc << " "),...); 
}

void test2_1()
{
    using type = make_index<7>::type;
    print_index_tuple(type());
}

int add(int a,int b)
{
    std::cout << a + b <<std::endl;
    return a + b;
}

void test2_2()
{
    int a = 9;
    auto r = std::reference_wrapper<int>(a);
    int& d = r;
    int&& e = std::forward<int>(r);

    d = 98;
    e = 90;

    std::cout << a << std::endl;

    auto f = std::bind(add,a,a);
    f();
}

class Test{
public:
	int *p;
    Test(){
		p = new int(9);
        printf("Test()\n");
    }
	Test(Test&& t) {
		this->p = t.p;
		t.p = nullptr;
		printf("Test(Test&& )\n");
	}
	Test(const Test& t) {
		this->p = t.p;
		printf("Test(Test& )\n");
	}
    ~Test(){
		if (p)
			delete p;
        printf("~Test()\n");
    }
	int operator=(int a)
	{
		*p = a;
		return a;
	}
	 operator int()
	{
		return *p;
	}

};

void f(Test t){
    
}

void test2_3()
{
    try{
		//int *p = reinterpret_cast<int*>(1);
		//*p = 9;
    }catch(...)
    {
        printf("exception!\n");
    }
    f(Test());
}
using std::any;
using std::any_cast;
void test_any()
{
    any a = 1;
    if(a.has_value())
    {
        printf("%s  %d\n",a.type().name(),any_cast<int>(a));
    }

    int *p = any_cast<int>(&a);
    cout << *p << endl;
}


Test&& retRr(Test &t)
{
	return std::move(t);
}

void test_rr()
{
	Test t;
	t = 90;
	Test t2 = retRr(t);
	cout << "t2 = "<< t2 << endl;
}

double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2)
{
	int max_len = nums1.size() + nums2.size();
	int len = max_len / 2 + 1;
	std::vector<int> arr3;
	arr3.reserve(len);
	int x = 0, y = 0;
	for (int i = 0; i < len; ++i)
	{
		if (x >= nums1.size()) {
			arr3.push_back(nums2[y++]); continue;
		}
		else if (y >= nums2.size()) {
			arr3.push_back(nums1[x++]); continue;
		}
		if (nums1[x] <= nums2[y])
		{
			arr3.push_back(nums1[x++]);
		}
		else {
			arr3.push_back(nums2[y++]);
		}
	}
	if (max_len % 2 != 0)
		return arr3[len - 1];
	else
		return (((double)arr3[len - 1]) + ((double)arr3[len - 2])) / 2.0;
}

double findMedianSortedArrays2(std::vector<int>& nums1, std::vector<int>& nums2)
{
	int m = nums1.size();
	int n = nums2.size();
	if (m > n) { // to ensure m<=n
		std::vector<int> temp = std::move(nums1); nums1 = std::move(nums2); nums2 = std::move(temp);
		int tmp = m; m = n; n = tmp;
	}
	int iMin = 0, iMax = m, halfLen = (m + n + 1) / 2;
	while (iMin <= iMax) {
		int i = (iMin + iMax) / 2;
		int j = halfLen - i;
		if (i < iMax && nums2[j - 1] > nums1[i]) {
			iMin = i + 1; // i is too small
		}
		else if (i > iMin && nums1[i - 1] > nums2[j]) {
			iMax = i - 1; // i is too big
		}
		else { // i is perfect
			int maxLeft = 0;
			if (i == 0) { maxLeft = nums2[j - 1]; }
			else if (j == 0) { maxLeft = nums1[i - 1]; }
			else { maxLeft = std::max(nums1[i - 1], nums2[j - 1]); }
			if ((m + n) % 2 == 1) { return maxLeft; }

			int minRight = 0;
			if (i == m) { minRight = nums2[j]; }
			else if (j == n) { minRight = nums1[i]; }
			else { minRight = std::min(nums2[j], nums1[i]); }

			return (maxLeft + minRight) / 2.0;
		}
	}
	return 0.0;
}

void longestPalindrome(std::string s) {
	int x = 0,y = 0,max_len = 0;
	int m,n,len;
	for(int i = 0;i < s.size();++i)
	{
		if(s[i] == s[i+1])
		{
			n = i+2;
			m = i-1;
			len = 2;
			while(n >= x)
			{
				if(m >= s.size())
				{
					if(len > max_len)
					{
						max_len = len;
						y = m - 1;
					}
					goto end;
				}
				if(s[m] == s[n])
				{
					len += 2;
				}
				--n;
				++m;
			}
			if(len > max_len)
			{
				max_len = len;
				y = m;
			}
		end:
			++x;
			i = x;
		}else if(s[i] == s[i+2])
		{
			n = i+3;
			m = i-1;
			len = 3;
			while(n >= x)
			{
				if(m >= s.size())
				{
					if(len > max_len)
					{
						max_len = len;
						y = m - 1;
					}
					goto end2;
				}
				if(s[m] == s[n])
				{
					len += 2;
				}
				--n;
				++m;
			}
			if(len > max_len)
			{
				max_len = len;
				y = m;
			}
		end2:
			++x;
			i = x;
		}
	}
	printf("%d  %d",x,y);
}

auto init()
{
    return  wws::CreateTFArray( 
									CREATE_TEST_FUNC(test2_1),
                                    CREATE_TEST_FUNC(test2_2),
                                    CREATE_TEST_FUNC(test2_3),
                                    CREATE_TEST_FUNC(test_any),
									CREATE_TEST_FUNC(test_rr),
									CREATE_TEST_FUNC(findMedianSortedArrays),
									CREATE_TEST_FUNC(findMedianSortedArrays2),
									CREATE_TEST_FUNC(longestPalindrome)
	);
}

}// namespace test2

#endif //__WWS_TEST2_H__