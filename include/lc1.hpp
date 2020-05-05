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
#include <comm.hpp>
#include <gray.hpp>
#include <serialization.hpp>
#include <json.hpp>
#include <list>

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
						if (m2[temp] > m1[temp]) //遇到符合的单词 但是超了
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
					else {//遇到了错误的单词
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
	int search(std::vector<int> nums, int target) {
		if (nums.size() == 1 && nums[0] == target) return 0;
        int b = 0,e = nums.size() - 1,stage = 0;
		while(b < e)
		{
			int m = (b + e) / 2;
			if (nums[b] == target) return b;
			if (nums[e] == target) return e;
			if (nums[m] == target) return m;
			if (m == b || m == e) break;
			if(!stage)
			{
				if (nums[m] > nums[0]) // 0 - m order
				{
					if (nums[0] < target && nums[m] > target)
					{
						stage = 1;
						e = m;
					}
					else {
						b = m;
					}
				}
				else
				{
					if (nums[m] < target && nums[e] > target)
					{
						stage = 1;
						b = m;
					}
					else {
						e = m;
					}
				}
			}else{
				if (target > nums[m])
				{
					b = m;
				}
				else {
					e = m;
				}
			}
		}
		return -1;
    }
	
	void test_search() {
        dbg(search({3,4,5,0,1,2},3));
		dbg(search({ 0,1,2,3,4,5 }, 3));
		dbg(search({ 4,5,0,1,2,3 }, 7));
		dbg(search({ 4,5,6,7,8,2,3 }, 2));
		dbg(search({ 1 }, 1));
    }
	template<bool left>
	int searchRange_find(std::vector<int>& nums, int target, int b_ = 0)
	{
		int b = b_;
		int e = nums.size() - 1;
		while (b <= e)
		{
			int mid = (b + e) / 2;
			if (nums[mid] == target)
			{
				if constexpr (left) {
					if (mid > 0)
					{
						if (nums[mid - 1] == target)
						{
							e = mid - 1;
						}
						else {
							return mid;
						}
					}
					else {
						return mid;
					}
				}
				else {//right
					if (mid < nums.size() - 1)
					{
						if (nums[mid + 1] == target)
						{
							b = mid + 1;
						}
						else {
							return mid;
						}
					}
					else {
						return mid;
					}
				}

			}
			else if (nums[mid] > target)
			{
				e = mid - 1;
			}
			else {
				b = mid + 1;
			}
		}
		return -1;
	}

	std::vector<int> searchRange(std::vector<int>& nums, int target) {
		std::vector<int> res = { -1,-1 };
		res[0] = searchRange_find<true>(nums, target);
		if (res[0] == -1)
			return res;
		res[1] = searchRange_find<false>(nums, target, res[0]);
		return res;
	}

	void test_searchRange()
	{
		typedef std::vector<int> V;
		V v1 = { 1,4,6,7,7,7,7,7,8,9 };
		dbg(searchRange(v1,7));
		V v2 = { 5,7,7,8,8,10 };
		dbg(searchRange(v2, 6));
	}

	void test_big_endina()
	{
		std::cout << std::boolalpha;
		std::cout << wws::big_endian() << std::endl;
	}

	void test_reverse_byte()
	{
		int a = 0x01020304;
		printBin(a);
		a = wws::reverse_byte(a);
		printBin(a);

		short b = 0x1122;
		printBin(b);
		b = wws::reverse_byte(b);
		printBin(b);

		char c = 8;
		printBin(c);
	}

	int searchInsert(std::vector<int> nums, int target) {
		int b = 0;
		int e = nums.size() - 1;
		if (target < nums[b])
			return 0;
		if (target > nums[e])
			return e + 1;
		while (b < e)
		{
			//std::cout << b << " " << e << std::endl;
			if (nums[b] == target)
				return b;
			if (nums[e] == target)
				return e;
			if (e - b == 1)
			{
				if (nums[b] < target && nums[e] > target)
				{
					return e;
				}
			}
			int m = (b + e) / 2;
			if (nums[m] == target)
			{
				return m;
			}
			else if (nums[m] > target) {
				e = m;
			}
			else {
				b = m;
			}
		}
		return 0;
	}

	void test_searchInsert()
	{
		dbg(searchInsert({ 1,3,5,6 }, 3));
	}

	void test_parser()
	{
		std::string s1 = "189";
		std::string s2 = "-902";
		std::string s3 = "1.89";
		std::string s4 = "4294967295";
		std::string s5 = "ff";

		dbg(wws::parser<int>(s1));
		dbg(wws::parser<int>(s2));
		dbg(wws::parser<float>(s3));
		dbg(wws::parser<unsigned int>(s4));
		dbg(wws::parser<short>(s1));
		dbg(wws::parser<int>(s5,16));
	}

	void test_seilza_to()
	{
		class MyClass
		{
		public:
			int a;
			float b;
			std::string c;
			long d;
		};

		MyClass mc{ 100,20.78f,"wws",988888 };
		std::string res = wws::seilza_to_line(mc, &MyClass::a, &MyClass::b, &MyClass::c, &MyClass::d);
		dbg(res);
		
		MyClass mc2;

		wws::seilza_form_line(res, mc2, &MyClass::a, &MyClass::b, &MyClass::c, &MyClass::d);

		dbg(mc2.a);
		dbg(mc2.b);
		dbg(mc2.c);
		dbg(mc2.d);

		dbg(wws::is_std_list<int>::val);
		dbg(wws::is_std_list<std::vector<int>>::val);

		const std::string arr = "[90,321,56,32]";
		std::vector<int> vs;
		wws::parser_stl(arr,vs);

		std::string arr_res = wws::to_string(vs);
		dbg(arr_res);


		class MyClass2
		{
		public:
			int a;
			float b;
			std::string c;
			std::vector<int> e;
			std::list<std::string> f;
			long d;
		};

		MyClass2 mc3;

		mc3.a = 90;
		mc3.b = 1.987f;
		mc3.c = "asdajsid";
		mc3.e = { 23,321,321412,1903 };
		mc3.f = { "go","ahead","win" };
		mc3.d = 31231213;

		std::string res2 = wws::seilza_to_line(mc3, &MyClass2::a, &MyClass2::b, &MyClass2::c, &MyClass2::d, &MyClass2::e, &MyClass2::f);
	
		std::cout << res2;

		MyClass2 mc4;

		wws::seilza_form_line(res2,mc4, &MyClass2::a, &MyClass2::b, &MyClass2::c, &MyClass2::d, &MyClass2::e, &MyClass2::f);

		 dbg(mc4.a);
		 dbg(mc4.b);
		 dbg(mc4.c);
		 dbg(mc4.d);
		 dbg(mc4.e);
		 dbg(mc4.f);
	}

	void test_json()
	{
		std::string s = "{\n"
			"\"gh\":   89 ,\n"
			"\"ju\":  80 ,\n"
			"\"lo\":        \"2222\"\n"
			"}";
		wws::Json j;
		wws::Json j2(s);

		std::string s2 = "{\"a\" : 89 , \"b\" : 80 , \"c\" : \"2222\" , \"d\" : {\"a\":89,\"b\":80,\"c\":\"2222\",\"nc\" : 90.0, \"bg\"  :  {\"go\": 90.90,\"jj\"\n  :\n  \"oooo\" } }, \"arr\" : [ 1 , 2 , 32 , 90 , 8989 ]  }";
		wws::Json j3(s2);

		dbg(j3.get<int>("a"));
		dbg(j3.get<std::string>("c"));
		std::string temp = "c";
		dbg(j3.get_str(temp));
		dbg(j3.get_obj("d").get<float>("nc"));
		dbg(j3.get_obj("d").get_obj("bg").get<float>("go"));

		dbg(j3.get_arr<int>("arr"));

		std::vector<std::string> v1 = { "123123","wws","wt","love","you" };
		
		std::string res = wws::to_string(v1, true);
		dbg(res);
		std::vector<std::string> res_v;
		wws::parser_stl(res, res_v);
		dbg(res_v);

		std::string res2 = wws::to_string(v1);
		dbg(res2);
		std::vector<std::string> res_v2;
		wws::parser_stl(res2, res_v2);
		dbg(res_v2);
		
		j3.put("add", 78);
		j3.put("adds", "wws");
		std::vector<int> v2 = { 1009,23,2312,231231,4213 };
		j3.put("arr2", v1);
		j3.put("arr3", v2);

		j3.put("j2", std::move(j2));

		auto j3_str = dbg(j3.to_string());

		wws::Json j4(j3_str);

		auto j4_str = dbg(j4.to_string());

		dbg(j4.has_key("adds"));
		dbg(j4.has_key("j2"));
		dbg(j4.has_key("j3"));

		auto new_j2 = j4.detach_obj("j2");

		std::string s5 = "{\"a\":-89.90,\"b\": -90,   \"c\": true }";

		wws::Json j5(s5);

		dbg(j5.get<float>("a"));
		dbg(j5.get<int>("b"));
		dbg(j5.get<bool>("c"));

		std::string j5_str = dbg(j5.to_string());

		wws::Json new_j5(j5_str);

		dbg(new_j5.get<bool>("c"));

		try {
			std::string s6 = "{\"a\": 90,"": }";
			wws::Json j6(s6);
		}
		catch (wws::BadJsonErr e)
		{
			dbg(e.what());
		}

	}

	bool isValidSudoku(std::vector<std::vector<char>>& board) {
        char rows[9][9] = {0};
		char cols[9][9] = {0};
		char boxs[9][9] = {0};
		for(int y = 0;y < 9;++y)
		{
			for (int x = 0; x < 9;++x)
			{
				char c = board[y][x];
				int v = (int)c - 48 - 1;
				if(c != '.')	
				{
					rows[x][v] += 1;
					cols[y][v] += 1;
					int box_idx = ((x / 3) % 3) + (y / 3) * 3;
					boxs[ box_idx ][v] += 1;
					if(rows[x][v] > 1 || cols[y][v] > 1 || boxs[ box_idx ][v] > 1)
						return false;
				}
			}
		}
		return true;
    }

	void test_isValidSudoku() 
	{
    	auto v = std::vector({
  			std::vector({'5','3','.','.','7','.','.','.','.'}),
  			std::vector({'6','.','.','1','9','5','.','.','.'}),
  			std::vector({'.','9','8','.','.','.','.','6','.'}),
  			std::vector({'8','.','.','.','6','.','.','.','3'}),
  			std::vector({'4','.','.','8','.','3','.','.','1'}),
  			std::vector({'7','.','.','.','2','.','.','.','6'}),
  			std::vector({'.','6','.','.','.','.','2','8','.'}),
  			std::vector({'.','.','.','4','1','9','.','.','5'}),
  			std::vector({'.','.','.','.','8','.','.','7','9'})
		});
		auto v2 = std::vector({
  			std::vector({'8','3','.','.','7','.','.','.','.'}),
			std::vector({'6','.','.','1','9','5','.','.','.'}),
			std::vector({'.','9','8','.','.','.','.','6','.'}),
			std::vector({'8','.','.','.','6','.','.','.','3'}),
			std::vector({'4','.','.','8','.','3','.','.','1'}),
			std::vector({'7','.','.','.','2','.','.','.','6'}),
			std::vector({'.','6','.','.','.','.','2','8','.'}),
			std::vector({'.','.','.','4','1','9','.','.','5'}),
			std::vector({'.','.','.','.','8','.','.','7','9'})
		});

		dbg(isValidSudoku(v));
		dbg(isValidSudoku(v2));
	}

	namespace SolveSudoku{

	bool is_valid(std::vector<std::vector<char>>& board,int row,int col,char c)
	{
		for(int i = 0;i < 9;++i)
		{
			if(board[row][i] == c)
				return false;
			if(board[i][col] == c)
				return false;
			if(board[row / 3 * 3 + i / 3][(col / 3) * 3 + (i % 3)] == c)
				return false;
		}
		return true;
	}

	bool back_trace(std::vector<std::vector<char>>& board,int row,int col)
	{
		if(col == 9)
		{
			row += 1;col = 0;
			if(row == 9)
				return true;
		}
		if(board[row][col] != '.')
			return back_trace(board,row,col + 1);
		
		for(char c = '1';c <= '9';++c)
		{
			if(!is_valid(board,row,col,c))
				continue;
			board[row][col] = c;
			if(back_trace(board,row,col + 1))
				return true;
			board[row][col] = '.';
		}
		return false;
	}

	void solveSudoku(std::vector<std::vector<char>>& board) {
        back_trace(board,0,0);
	}

	void test_solveSudoku() 
	{
    	auto v = std::vector({
  			std::vector({'5','3','.','.','7','.','.','.','.'}),
  			std::vector({'6','.','.','1','9','5','.','.','.'}),
  			std::vector({'.','9','8','.','.','.','.','6','.'}),
  			std::vector({'8','.','.','.','6','.','.','.','3'}),
  			std::vector({'4','.','.','8','.','3','.','.','1'}),
  			std::vector({'7','.','.','.','2','.','.','.','6'}),
  			std::vector({'.','6','.','.','.','.','2','8','.'}),
  			std::vector({'.','.','.','4','1','9','.','.','5'}),
  			std::vector({'.','.','.','.','8','.','.','7','9'})
		});
		

		solveSudoku(v);

		dbg(v);
	}

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
			CREATE_TEST_FUNC(test_longestValidParentheses),
			CREATE_TEST_FUNC(test_search),
			CREATE_TEST_FUNC(test_searchRange),
			CREATE_TEST_FUNC(test_big_endina),
			CREATE_TEST_FUNC(test_reverse_byte),
			CREATE_TEST_FUNC(test_searchInsert),
			CREATE_TEST_FUNC(test_parser),
			CREATE_TEST_FUNC(test_seilza_to),
			CREATE_TEST_FUNC(test_json),
			CREATE_TEST_FUNC(test_isValidSudoku),
			CREATE_TEST_FUNC(SolveSudoku::test_solveSudoku)
        );
    }
}


#endif //HELLO_LC1_H
