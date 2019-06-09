//
// Created by dell on 2019/6/6.
//

#ifndef HELLO_LC1_H
#define HELLO_LC1_H

#include <iostream>
#include <dbg.hpp>
#include <vector>
#include <TestFunc.hpp>
#include <print_stl.hpp>

namespace lc1{

   
    int removeDuplicates(std::vector<int>& nums) {
        if(nums.empty())
            return 0;
        int n = nums.size();
		int i = 0;
		for (int j = 1; j < n; ++j)
		{
			if (nums[i] != nums[j])
				nums[++i] = nums[j];
		}
		return i + 1;
    }



    void test_removeDuplicates()
    {
        std::vector<int> in1 = {1,1,2};
        std::vector<int> in2 = {0,0,1,1,1,2,2,3,3,4};
        dbg(in1);
        dbg(removeDuplicates(in1));
        dbg(in1);
        dbg(in2);
        dbg(removeDuplicates(in2));
        dbg(in2);
    }
    int removeElement(std::vector<int>& nums, int val) {
		if (nums.empty())
			return 0;
		int n = nums.size();
		int i = 0, j = n - 1;
		while (i < j)
		{
			if (nums[i] == val)
			{
				if (nums[j] == val) { --j; continue; }
				nums[i] = nums[j];
				if (j + 1 <= i) return n - j;
				--j;
			}
			++i;
		}
		if (nums[j] != val) return n - (n - (j + 1));
		return n - (n - j);
    }
    void test_removeElemens() {
        std::vector<int> in1 = { 3,2,2,3 };
		std::vector<int> in3 = { 3,2,2,3,5,2 };
		std::vector<int> in2 = { 0,1,2,2,3,0,4,2 };
		dbg(std::make_tuple(in1,3));
		int res = removeElement(in1,3);
        dbg(res);
        dbg(in1);
		dbg(std::make_tuple(in2, 2));
		res = removeElement(in2, 2);
		dbg(res);
		dbg(in2);
		dbg(std::make_tuple(in3, 2));
		res = removeElement(in3, 2);
		dbg(res);
		dbg(in3);
    }

	int strStr(std::string haystack, std::string needle) {
		if (needle.size() == 0)
			return 0;
		if (needle.size() > haystack.size())
			return -1;
		int m = haystack.size(), n = needle.size();
		int i = 0, j = 0;
		for (; i < m; ++i)
		{
			Go:
			if (haystack[i] == needle[j])
			{
				int t_i = i + 1;
				++j;
				while (j < n)
				{
					if (t_i >= m)
					{
						return -1;
					}
					if (haystack[t_i] != needle[j])
					{
						i = i + 1; j = 0; goto Go;
					}
					++t_i; ++j;
				}
				return i;
			}
		}
		return -1;
	}

	void test_strStr()
	{
		dbg(strStr("aaa", "aaaa"));
		dbg(strStr("aabc", "abc"));
		dbg(strStr("aaa", "aaa"));
		dbg(strStr("aaa", "ac"));
	}

	int divide(int dividend, int divisor) {
		if (divisor == 0) return 0;
		bool nv = (dividend ^ divisor) > 0 ? false : true;
		if (dividend == divisor) nv = false;

		long long res = 0;
		long long lldividend = abs((long long)dividend);
		long long lldivisor = abs((long long)divisor);
		long long t = 1;
		while (lldividend >= lldivisor << 1 && (lldivisor << 1) > 0)
		{
			t <<= 1;
			lldivisor <<= 1;
		}
		while (t > 0 && lldividend > 0)
		{
			if (lldividend >= lldivisor)
			{
				lldividend -= lldivisor;
				res += t;
			}
			lldivisor >>= 1;
			t >>= 1;
		}
		res = nv ? -res : res;
		if (res > 0x7fffffff)
		{
			return std::numeric_limits<int>::max();
		}
		return (int)res;
	}

	void test_divide()
	{
		dbg(divide(10, 3));
		dbg(divide(7, -3));
		dbg(divide(-70, -3));
		dbg(divide(-78, 2));
		dbg(divide(-78, -1));
		dbg(divide(-78, 1));
		dbg(divide(78, -1));
		dbg(divide(2147483647, 2));
		dbg(divide(-2147483648, 1));
		dbg(divide(1, 1));
	}

    auto init()
    {
        return  wws::CreateTFArray(
            CREATE_TEST_FUNC(test_removeDuplicates),
            CREATE_TEST_FUNC(test_removeElemens),
			CREATE_TEST_FUNC(test_strStr),
			CREATE_TEST_FUNC(test_divide)
        );
    }
}


#endif //HELLO_LC1_H
