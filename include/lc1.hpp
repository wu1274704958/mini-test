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

    void move_left(std::vector<int>& nums,int b,int r_len) {
        for (int i = b; i < r_len; ++i) {
            nums[i - 1] = nums[i];
        }
    }
    int removeDuplicates(std::vector<int>& nums) {
        if(nums.empty())
            return 0;
        int r_len = nums.size();
        int last = nums[0];
        for (int i = 1; i < r_len; ++i) {
            if(nums[i] == last)
            {
                move_left(nums,i,r_len);
                i -= 1; r_len -= 1;
            }
            last = nums[i];
        }
        return r_len;
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
