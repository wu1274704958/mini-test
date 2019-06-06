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

    auto init()
    {
        return  wws::CreateTFArray(
            CREATE_TEST_FUNC(test_removeDuplicates)
        );
    }
}


#endif //HELLO_LC1_H
