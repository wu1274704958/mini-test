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
#include <map>
#include <unordered_map>

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

    bool map_same(std::map<std::string*,int> m1,std::map<std::string*,int> m2)
    {
        if(m1.size() != m2.size())
            return false;
        for(auto it = m1.begin();it != m1.end();++it)
        {
            if(m2.find(it->first) != m2.end())
            {
                if(m2[it->first] != it->second)
                    return  false;
            }else
                return  false;
        }
        return true;
    }

    int same_words(std::string &s,int i,std::vector<std::string>& words)
    {
        int wl = words[0].size();
        if(i + wl > s.size())
            return -1;
        int j,n,wi;
        for(j = 0,n = i,wi = 0;wi < wl;++n,++wi)
        {
            if(s[n] != words[j][wi])
            {
                if(j == words.size() - 1)
                    return -1;
				++j; n = i - 1; wi = -1;
                continue;
            }
        }
        return j;
    }

	int find_first(std::string& s, std::vector<std::string>& words)
	{
		for (int i = 0;i < words.size();++i)
		{
			if (s == words[i])
				return i;
		}
		return -1;
	}

    std::vector<int> findSubstring(std::string s, std::vector<std::string>&& words) {
        if(s.empty() || words.empty()) return {};
        int wl = words[0].size();
        int n = s.size();
        int wn = words.size();
        std::vector<int> res;
        std::map<std::string*,int> m1;
        for(auto& w : words)
        {
			int v = find_first(w, words);
			std::string* t = &words[v];
            if(m1.find(t) != m1.end())
            {
                int v = m1[t];
                m1[t] = v + 1;
            } else
                m1[t] = 1;
        }
        int sl = 0;
        int bi = 0;
        std::map<std::string*,int> m2;
        for(int i = 0;i < n;)
        {
            if(sl == wn)
            {
                if(map_same(m1,m2))
                    res.push_back(bi);
                m2.clear();
                sl = 0;
				i = bi + 1;
				bi = i;
            }
            int v = same_words(s,i,words);
            if(v == -1)
            {
				m2.clear(); 
				sl = 0;
				i = bi + 1;
				bi = i;
                continue;
            }
            if(m2.find(&words[v]) != m2.end())
            {
                int temp = m2[&words[v]];
                m2[&words[v]] = temp + 1;
            }else{
                m2[&words[v]] = 1;
            }
			i += wl;
			++sl;
        }
		if (sl == wn && map_same(m1, m2))
			res.push_back(bi);
        return res;
    }

	std::vector<int> findSubstring2(std::string s, std::vector<std::string>&& words) {
		if (s.empty() || words.empty()) return {};
		int wl = words[0].size();
		int n = s.size();
		int wn = words.size();
		std::vector<int> res;
		std::unordered_map<std::string, int> m1;
		for (auto& w : words)
		{
			if (m1.find(w) != m1.end())
			{
				++m1[w];
			}
			else
				m1[w] = 1;
		}

		for (int i = 0; i < wl; ++i)
		{
			std::unordered_map<std::string, int> m2;
			int hwn = 0;
			
			for (int b = i; b <= n - wl * wn; b += wl)
			{
				bool has_removed = false;
				while (hwn < wn)
				{
					std::string temp = s.substr(b + hwn * wl, wl);
					if (m1.find(temp) != m1.end())
					{
						if (m2.find(temp) != m2.end())
							++m2[temp];
						else
							m2[temp] = 1;
						if (m2[temp] > m1[temp]) //�������ϵĵ��� ���ǳ���
						{
							int remove_num = 0;
							has_removed = true;
							while (m2[temp] > m1[temp])
							{
								auto will_remo = s.substr(b + remove_num * wl, wl);
								--m2[will_remo];
								++remove_num;
							}
							hwn = hwn - remove_num + 1;
							b = b + (remove_num - 1) * wl;
							break;
						}
					}
					else {//�����˴���ĵ���
						m2.clear();
						b = b + hwn * wl;
						hwn = 0;
						break;
					}
					++hwn;
				}
				if (hwn == wn)
				{
					res.push_back(b);
				}
				if (hwn > 0 && !has_removed) 
				{
					auto ts = s.substr(b, wl);
					--m2[ts];
					--hwn;
				}
			}
		}

		return res;
	}


    void test_findSubstring()
    {
       
		dbg(findSubstring2("barfoothefoobarman", { "bar", "foo" }));

		dbg(findSubstring2("wordgoodgoodgoodbestword", { "word", "good", "best", "word" }));

		dbg(findSubstring2("abacbabacba", { "ab", "ba", "ac", "ba" }));

		dbg(findSubstring2("ababaab", { "ab", "ba", "ba" }));
	
		dbg(findSubstring2("barfoofoobarthefoobarman", { "bar", "foo", "the" }));

		dbg(findSubstring2("aaaaaaaa", { "aa", "aa", "aa" }));
    }

	void swap(std::vector<int>& c, int i, int j)
	{
		if (i == j) return;
		int temp = c[i];
		c[i] = c[j];
		c[j] = temp;
	}

	void reverse(std::vector<int>& nums, int s_i)
	{
		int e = nums.size() - 1;
		if (s_i == e) return;
		int l = ((e - s_i) / 2) + 1;
		for (int i = 0; i < l; ++s_i, --e,++i)
		{
			swap(nums,s_i, e);
		}
	}

	void nextPermutation(std::vector<int>& nums) {
		int len = nums.size();
		int i = len - 2;
		while (i >= 0 && nums[i + 1] <= nums[i])
			--i;

		if (i >= 0)
		{
			int j = len - 1;
			while (j >= 0 && nums[j] <= nums[i])
				--j;
			swap(nums, i, j);
			reverse(nums, i + 1);
			return;
		}
		std::reverse(std::begin(nums), std::end(nums));
	}

	void test_nextPermutation()
	{
		std::vector<int> _1 = { 1, 2, 3, 4 };
		nextPermutation(_1);
		dbg(_1);
		nextPermutation(_1);
		dbg(_1);
		nextPermutation(_1);
		dbg(_1);
		nextPermutation(_1);
		dbg(_1);
		nextPermutation(_1);
		dbg(_1);
	}

	int longestValidParentheses(std::string s) {
		int dpl = s.size();
		int* dp = new int[dpl];
		memset(dp, 0, sizeof(int) * dpl);
		int res = 0;
		for (int i = 1; i < dpl; ++i)
		{
			if (s[i] == ')')
			{
				if (s[i - 1] == '(')
				{
					if (i > 1)
					{
						dp[i] = dp[i - 2] + 2;
					}
					else {
						dp[i] = 2;
					}
				}
				else if (i > 1 && s[i - 1] == ')' && (i - dp[i - 1] - 1) >= 0 && s[i - dp[i - 1] - 1] == '(')
				{
					dp[i] = dp[i - 1] + ( (i - dp[i - 1]) >= 2 ? dp[i - dp[i - 1] - 2] : 0 ) + 2;
				}
			}
			if (dp[i] > res) res = dp[i];
		}
		delete[] dp;
		return res;
	}

	void test_longestValidParentheses()
	{
		auto f = longestValidParentheses;
		dbg(f("()()()(()))"));
		dbg(f("()()"));
		dbg(f("()(()"));
		dbg(f(")))(((("));
		dbg(f("(((()))"));
		dbg(f("(()())"));
	}

    auto init()
    {
        return  wws::CreateTFArray(
            CREATE_TEST_FUNC(test_removeDuplicates),
            CREATE_TEST_FUNC(test_removeElemens),
			CREATE_TEST_FUNC(test_strStr),
			CREATE_TEST_FUNC(test_divide),
			CREATE_TEST_FUNC(test_findSubstring),
			CREATE_TEST_FUNC(test_nextPermutation),
			CREATE_TEST_FUNC(test_longestValidParentheses)
        );
    }
}


#endif //HELLO_LC1_H