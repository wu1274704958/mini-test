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
        if(nums.empty())
            return 0;
        int n = nums.size();
        if(n == 1) return 1;
        int i = 0,j = n -1;
        while(i < j)
        {
            dbg(j);
            dbg(i);
            if(nums[i] == val)
            {
                if(nums[j] == val){ --j;continue;}
                nums[i] = nums[j];
                if(j + 1 >= i) return n -j;
                --j;
            }
            ++i;
        }
        return n - j;
    }
    void test_removeElemens() {
        std::vector<int> in1 = {3,2,2,3};
        int res = removeElement(in1,3);
        dbg(res);
        dbg(in1);
    }
    auto init()
    {
        return  wws::CreateTFArray(
            CREATE_TEST_FUNC(test_removeDuplicates),
            CREATE_TEST_FUNC(test_removeElemens)
        );
    }
}


#endif //HELLO_LC1_H
